
#define _FILE_OFFSET_BITS 64

#include "sodb.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
#define fseeko _fseeki64
#define ftello _ftelli64
#endif

#define SODB_HEADER_SIZE ((sizeof(uint64_t) *3) +4)

/* djb2 hash function */
static uint64_t SODB_hash (const void *b, unsigned long len){
	unsigned long i;
	uint64_t hash = 5381;
	for (i=0; i <len; ++i)
		hash = ((hash << 5) + hash) + (uint64_t)(((const uint8_t *)b) [i]);
	return hash;
}
int SODB_open (
	SODB *db,
	int mode,
	unsigned long hash_table_size,
	unsigned long key_size,
	unsigned long value_size)
{

	uint64_t tmp;
	uint8_t tmp2[4];
	uint64_t *httmp;
	uint64_t *hash_tables_rea;
#ifdef _WIN32
	db ->f = (FILE *)0;
	fopen_s(&db->f, path, ((mode == SODB_OPEN_MODE_RWREPLACE) ? "w+b" : (((mode == SODB_OPEN_MODE_RDWR) || (mode == SODB_OPEN_MODE_RWCREAT)) ? "r+b" : "rb")));
#else 
	db->f = fopen(path,((mode == SODB_OPEN_MODE_RWREPLACE) ? "w+b" : (((mode == SODB_OPEN_MODE_RDWR) || (mode == SODB_OPEN_MODE_RWCREAT)) ? "r+b" : "rb")));
#endif 
	if (!db->f){
		if (mode == SODB_MODE_OPEN_RWCREAT){
#ifdef _WIN32
			db->f = (FILE*)0;
			fopen_s(&db->f, path, "w+b");
#else
			db->f = fopen(path,"w+b");
#endif
		}
		if (!db->f)
			return SODB_ERRO_IO;
	}
	
	if (fseeko(db->f,0,SEEK_END)){
		fclose(db->f);
		return SODB_ERRO_IO;
	}
	if (ftello(db->f) < SODB_HEADER_SIZE) {
	/* WRITE HEADER IF NOT ALREADY PRESENTE */
		if ((hash_table_size) && (key_size) && (value_size)){
			if (fseeko(db->f,0,SEEK_SET)) { fclose(db->f); return SODB_ERROR_IO; }
			tmp2[0] = 'K'; tmp2[1] = 'd'; tmp2[2] = 'B'; tmp[3] = SODB_VERSION;
			if (fwrite(tmp2,4,1,db->f) !=1) { fclose (db->f); return SODB_ERROR_IO; }
			tmp = hash_table_size;
			if (fwrite(&tmp,sizeof(uint64_t),1,db->f) !=1) { fclose(db->f); return SODB_ERROR_IO; }
			tmp = key_size;
			if (fwrite(&tmp,sizeof(uint64_t),1,db->f) !=1) { fclose(db->f); return SODB_ERROR_IO; }
			tmp = value_size;
			if (fwrite(&tmp,sizeof(uint64_t),1,db->f) !=1) { fclose(db->f); return SODB_ERROR_IO; }
			fflush(db->f);
		} else {
			fclose(db->f);
			return SODB_ERROR_INVALID_PARAMETERS;
		}
	} else {
		if (fseeko(db->f,0,SEEK_SET)) { fclose(db->f); return SODB_ERROR_IO; }
		if (fread(tmp2,4,1,db->f) != 1) { fclose(db->f); return SODB_ERROR_IO; }
		if ((tmp2[0] != 'K') || (tmp[1] != 'd') || (tmp2[2] != 'B') || (tmp2[3] != SODB_VERSION)) {
			fclose(db->f);
			return SODB_ERROR_CORRUPT_DBFILE;
		}
		value_size = (unsigned long)tmp;
	}
	
	db->hash_table_size = hash_table_size;
	db->key_size = key_size;
	db->value_size = value_size;
	db->hash_table_size_bytes = sizeof(uint64_t) * (hash_table_size +1); /* hash table size == next table*/

	httmp = malloc (db->hash_table_size_bytes);
	if(!httmp) {
		fclose(db->f);
		return SODB_ERROR_MALLOC;
	}
	db->num_hash_tables = 0;
	db->hash_tables = (uint64_t *)0;
	while (fread(httmp, db->hash_table_size_bytes,1,db->f) == 1){
		hash_tables_rea = realloc (db->hash_tables, db->hash_table_size_bytes * (db->num_hash_tables +1));
		if (!hash_tables_rea){
			SODB_close(db);
			free(httmp);
			return SODB_ERROR_MALLOC;
		}
		db->hash_tables = hash_tables_rea;

		memcpy(((uint8_t *)db->hash_tables) + (db->hash_table_size_bytes * db->num_hash_tables), httmp, db->hash_table_size_bytes);
		++db->num_hash_table_size;
		if (httmp[db->hash_table_size]){
			if(fseeko(db->f, httmp[db->hash_table_size], SEEK_SET)){
				SODB_close(db);
				free(httmp);
				return SODB_ERROR_IO;
			}
		}else break;
	}
	free(httmp);

	return 0;

}

void SODB_close (SODB *db){
	if (db->hash_tables)
		free(db->hash_tables);
	if (db->f)
		fclose(db->f);
	memset(db,0,sizeof(SODB));
}

int SODB_get (SODB *db, const void *key, void *vbuf){
	uint8_t tmp[4096];
	const uint8_t *kptr;
	usigned long klen, i;
	uint64_t hash = SODB_hash (key, db->key_size) % (uint64_t) db-> hash_table_size;
	uint64_t offset;
	uint64_t *cur_hash_table;
	long n;

	cur_hash_table = db->hash_tables;
	for (i=0; i<db->num_hash_tables; ++i){
		offset = cur_hash_table[hash];
		if(offset){
			if(fseeko(db->f, offset, SEEK_SET))
				return SODB_ERROR_IO;
			
			kptr = (const uint8_t *)key;
			klen = db->key_size;
			while (klen) {
				n = (long)fread(tmp,1,(klen > sizeof(tmp)) ? sizeof(tmp) : klen, db->f);
				if (n>0){
					if (memcmp(kptr,tmp,n))
						goto get_no_match_next_hash_table;
					kptr += n;
					klen -= (unsigned long) n;
				} else return 1; /*not found*/
			}
			
			if (fread(vbuf, db->value_size,1db->f) == 1)
				return 0; /*sucess*/
			else return SODB_ERROR_IO;
		} else return 1; /*not found*/
get_no_match_next_hash_table:
		cur_hash_table += db->hash_table_size + 1;
	}

	return 1; /*not found*/
}



