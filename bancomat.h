#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

enum menu {SALDO = 's', VERSAMENTO = 'v', RICARICA = 'r', PRELIEVO = 'p', INFO_CONTO = 'i', LISTA_ENTRATE = 'e', LISTA_USCITE = 'u', EXIT = 'x', BONIFICO = 'b'};

typedef struct utente_s {
    char user[30];
    char nome[30];
    char cognome[30];
    char data_di_nascita[15];
    char city[30];
    char intestatario[60];
    int cvv[3];
    int num_carta[16];
    int codice_carta[4];
    char password[30];
    float saldo,entrate,uscite;
}utente_t;

typedef struct lista_movimenti
{
    char *data;
    char *ora;
    float importo;
}l_m;


struct node {
   struct lista_movimenti lm;
   struct node *next;
};

typedef struct node *Node;

struct list
{
    Node head;
    Node curN;
};

typedef struct list *List;


List createList();

void printEntrate(List l);

void printUscite(List l);

bool insertFirst(List l, struct lista_movimenti m);

bool isEmpty(List l);

void printListFileEntrate(List l, char *user, l_m m );

void printListFileUscite(List l, char *user, l_m m );

void readFileEntrate(char *user);

void readFileUscite(char *user);

void createFileList(char *user);

int pass_recovery(char *user);

int data_p_c(char *user, char *psw);

void lower(char *str);

int card_generator();

int new_pass(char *user, char *psw);

float visual_saldo(char *user);

int visual_conto (char *user);

char* data();

char *ora();

int vers(char *user, float importo);

int bonifico(char *user, char *bonuser, float importo);

int trovautente(char *user, char *psw);

int prelievo(char *user, float importo);

int visual_eus(char *user);