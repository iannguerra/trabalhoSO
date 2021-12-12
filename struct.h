#ifndef SOME_HEADER_GUARD_WITH_UNIQUE_NAME
#define SOME_HEADER_GUARD_WITH_UNIQUE_NAME

struct st_registro
{
    int id; // Os itens id e type da struct registro formam uma chave primária
    char type[300];
    char attributes[300]; // Informações amarradas à chave primária
};
typedef struct st_registro registro;

#endif