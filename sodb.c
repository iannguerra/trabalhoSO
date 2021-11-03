
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
			if (fseeko(db->f,0,SEEK_SET)) { fclose(db->f); return SODB_ERRO_IO; }
	
