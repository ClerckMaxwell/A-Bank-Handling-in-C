#include "bancomat.h"

List createList()
{
    List l = (List)malloc(sizeof(struct list));
    if (l != NULL)
    {
        l->head = NULL;
        l->curN = NULL;
    }
    else
        printf("Non e' stato possibile creare la lista\n");
    return l;
}

void printEntrate(List l)
{
    if (l != NULL)
    {
        Node n = l->head;
        if(n== NULL) 
            printf("\nAl momento in questa sessione non ci sono state entrate\n");
        while (n != NULL)
        {
            printf("Hai versato $ %.2f in data %s e ora %s\n", n->lm.importo, n->lm.data, n->lm.ora);
            n = n->next;
        }
    }
    else
        printf("Lista vuota\n");
}

void printUscite(List l)
{
    if (l != NULL)
    {
        Node n = l->head;
        if(n== NULL) 
            printf("\nAl momento in questa sessione non ci sono state uscite\n");
        while (n != NULL)
        {
            printf("Hai prelevato $ %.2f in data %s e ora %s\n", n->lm.importo, n->lm.data, n->lm.ora);
            n = n->next;
        }
    }
    else
        printf("Lista vuota\n");
}

bool insertFirst(List l, struct lista_movimenti m)
{
    Node node = (Node)malloc(sizeof(struct node));
    bool inserted = false;
    if (l != NULL && node != NULL)
    {
        node->lm.data = malloc((1 + strlen(m.data)) * sizeof(char));
        node->lm.ora = malloc((1 + strlen(m.ora)) * sizeof(char));
        strcpy(node->lm.data, m.data);
        strcpy(node->lm.ora, m.ora);
        node->lm.importo = m.importo;

        node->next = l->head;
        l->head = node;
        inserted = true;
    }
    else
        printf("Errore imprevisto\n");
    return inserted;
}


bool isEmpty(List l)
{
    return l == NULL || l->head == NULL;
}

void printListFileEntrate(List l, char *user, l_m m)
{
    char *buffer;
    buffer = malloc((2 + strlen(user) + strlen("entrate")) * sizeof(char));
    sprintf(buffer, "%s.%s", user, "entrate");
    FILE *file = fopen(buffer, "r+");
    free(buffer);
    buffer = malloc(200*sizeof(char));
    while(fgets(buffer, 200, file));
    if (!isEmpty(l))
    {
        Node curr = l->head;
        curr->lm.data = malloc((1 + strlen(m.data)) * sizeof(char));
        curr->lm.ora = malloc((1 + strlen(m.ora)) * sizeof(char));
        strcpy(curr->lm.data, m.data);
        strcpy(curr->lm.ora, m.ora);
        curr->lm.importo = m.importo;
        while (curr != NULL)
        {
            fprintf(file, "Hai caricato $ %.2f in data %s alle ore %s\n", curr->lm.importo, curr->lm.data, curr->lm.ora);
            curr = curr->next;
        }
    }
    fclose(file);
}

void printListFileUscite(List l, char *user, l_m m)
{
    char *buffer;
    buffer = malloc((2 + strlen(user) + strlen("Uscite")) * sizeof(char));
    sprintf(buffer, "%s.%s", user, "uscite");
    FILE *file = fopen(buffer, "r+");
    free(buffer);
    buffer = malloc(200*sizeof(char));
    while(fgets(buffer, 200, file));
    if (!isEmpty(l))
    {
        Node curr = l->head;
        curr->lm.data = malloc((1 + strlen(m.data)) * sizeof(char));
        curr->lm.ora = malloc((1 + strlen(m.ora)) * sizeof(char));
        strcpy(curr->lm.data, m.data);
        strcpy(curr->lm.ora, m.ora);
        curr->lm.importo = m.importo;
        while (curr != NULL)
        {
            fprintf(file, "Hai prelevato $ %.2f in data %s alle ore %s\n", curr->lm.importo, curr->lm.data, curr->lm.ora);
            curr = curr->next;
        }
    }
    fclose(file);
}

void readFileEntrate(char *user)
{
    char *buffer;
    buffer = malloc((2 + strlen(user) + strlen("entrate")) * sizeof(char));
    sprintf(buffer, "%s.%s", user, "entrate");
    FILE *file = fopen(buffer, "r");
    free(buffer);
    char lettura[200];
    int i = 0;
    while(fgets(lettura, 200, file))
    {
            printf("%s", lettura);
            i++;
    }
    if (i==0) printf("Non ci sono entrate storiche. Controllo se c'e' stata qualche entrata in questa sessione...\n");
    usleep(300000);
    fclose(file);
}

void readFileUscite(char *user)
{
    char *buffer;
    buffer = malloc((2 + strlen(user) + strlen("uscite")) * sizeof(char));
    sprintf(buffer, "%s.%s", user, "uscite");
    FILE *file = fopen(buffer, "r");
    free(buffer);
    char lettura[200];
    int i = 0;
    while(fgets(lettura, 200, file))
    {
            printf("%s", lettura);
            i++;
    }
    if (i==0) printf("Non ci sono uscite storiche. Controllo se c'e' stata qualche uscita in questa sessione...\n");
    
    usleep(300000);
    fclose(file);
}

void createFileList(char *user)
{
    char *buffer;
    buffer = malloc((2 + strlen(user) + strlen("entrate") * sizeof(char)));
    sprintf(buffer, "%s.%s", user, "entrate");
    FILE *file1 = fopen(buffer, "w");
    free(buffer);
    fclose(file1);
    buffer = malloc((2 + strlen(user) + strlen("uscite") * sizeof(char)));
    sprintf(buffer, "%s.%s", user, "uscite");
    FILE *file2 = fopen(buffer, "w");
    free(buffer);
    fclose(file2);
}

void lower(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
        str[i] = tolower(str[i]);
}

int data_p_c(char *user, char *psw)
{
    utente_t utente;
    strcpy(utente.password, psw);
    utente.saldo = (float)0;
    utente.entrate = (float)0;
    utente.uscite = (float)0;
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user")) * sizeof(char));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "w+");
    puts("Come ti chiami?(Solo nome)");
    fgets(utente.nome,29,stdin);
    if(strlen(utente.nome)>27)
        while('\n'!=getchar());
    utente.nome[strlen(utente.nome)-1]= '\0';

    puts("Cognome?");
    fgets(utente.cognome,29,stdin);
    if(strlen(utente.cognome)>27)
        while('\n'!=getchar());
    utente.cognome[strlen(utente.cognome)-1]= '\0';

    puts("Data di nascita? Scrivi come gg/mm/aa");
    scanf("%14s", utente.data_di_nascita);
    while('\n'!=getchar());

    puts("Dove sei nato?");
    fgets(utente.city,29,stdin);
    if(strlen(utente.city)>27)
        while('\n'!=getchar());
    utente.city[strlen(utente.city)-1]= '\0';

    if (fwrite(&utente, sizeof(utente_t), 1, fileuser) < 0)
        return 0;
    fclose(fileuser);
    return 1;
}

int card_generator(char *user)
{
    int i = 0;
    utente_t lettura;
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user")) * sizeof(char));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r+");
    fread(&lettura, sizeof(utente_t), 1, fileuser);
    srand(time(NULL));
    free(buffer);
    buffer = malloc(((1 + strlen(lettura.nome) + strlen(lettura.cognome)) * sizeof(char)));
    lettura.nome[strlen(lettura.nome)]='\0';
    lettura.cognome[strlen(lettura.cognome)]='\0';
    sprintf(buffer, "%s %s", lettura.nome, lettura.cognome);
    strcpy(lettura.intestatario, buffer);
    free(buffer);
    for (i = 0; i < 3; i++)
        lettura.cvv[i] = rand() % 10;
    for (i = 0; i < 16; i++)
        lettura.num_carta[i] = rand() % 10;
    for (i = 0; i < 4; i++)
        lettura.codice_carta[i] = rand() % 10;

    rewind(fileuser);
    if (fwrite(&lettura, sizeof(utente_t), 1, fileuser) < 0)
        return 0;
    else
    {
        printf("Numero carta: ");
        for (i = 0; i < 16; i++)
        {
            printf("%d", lettura.num_carta[i]);
            if ((i + 1) % 4 == 0)
            {
                printf(" ");
            }
        }
        printf("\n");
        printf("Intestatario : %s\nCVV = %d%d%d\nCodice carta : ", lettura.intestatario, lettura.cvv[0], lettura.cvv[1], lettura.cvv[2]);
        for (i = 0; i < 4; i++)
            printf("%d", lettura.codice_carta[i]);
        printf("\n");
    }
    fclose(fileuser);
    return 1;
}

int pass_recovery(char *user)
{
    char str[3];
    int fine = 0;
    puts("Sembra che tu non riesca ad accedere. Puoi riavere la tua password ma devi darmi le tue generalita'\nVuoi?");
    while (!fine)
    {
        scanf("%s", str);
        while ('\n' != getchar())
            ;
        lower(str);
        if (strcmp(str, "no") == 0)
            return -1;
        if (strcmp(str, "si") == 0)
            fine = 1;
        else
            puts("Non ho capito riprova");
    }
    char *buffer;
    char *buffercopia;
    buffercopia = malloc((1 + strlen(user) + strlen("copy")) * sizeof(char));
    buffer = malloc((1 + strlen(user) + strlen(".user")) * sizeof(char));
    strcpy(buffercopia, user);
    strcat(buffercopia, "copy");
    strcpy(buffer, user);
    strcat(buffer, ".user");
    data_p_c(buffercopia, " "); //l'user passato qui sara' 'user'copy, e tutto sara' salvato in 'user'copy.user
    strcat(buffercopia, ".user");
    FILE *filecopia = fopen(buffercopia, "r");
    FILE *fileuser = fopen(buffer, "r");
    utente_t vero;
    utente_t copia;
    fread(&vero, sizeof(utente_t), 1, fileuser);
    fread(&copia, sizeof(utente_t), 1, filecopia);
    lower(vero.nome);
    lower(copia.nome);
    lower(vero.cognome);
    lower(copia.cognome);
    lower(vero.city);
    lower(copia.city);
    if (strcmp(vero.nome, copia.nome) != 0)
        return 0;
    if (strcmp(vero.cognome, copia.cognome) != 0)
        return 0;
    if (strcmp(vero.data_di_nascita, copia.data_di_nascita) != 0)
        return 0;
    if (strcmp(vero.city, copia.city) != 0)
        return 0;
    fclose(fileuser);
    fclose(filecopia);
    remove(buffercopia);
    free(buffercopia);
    free(buffer);
    return 1;
}

int new_pass(char *user, char *psw)
{
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user")) * sizeof(char));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fp = fopen(buffer, "r+");
    if (!fp)
        return 0;
    utente_t utente_temp;
    fread(&utente_temp, sizeof(utente_t), 1, fp);
    char *old;
    old = malloc((1 + strlen(utente_temp.password)) * sizeof(char));
    strcpy(old, utente_temp.password);
    strcpy(utente_temp.password, psw);
    char *oldbuff;
    oldbuff = malloc((2 + strlen(old) + strlen(user)) * sizeof(char));
    sprintf(oldbuff, "%s.%s", user, old);
    free(buffer);
    buffer = malloc((2 + strlen(user) + strlen(psw)) * sizeof(char));
    sprintf(buffer, "%s.%s", user, psw);
    printf("Vecchia password : %s\nNuova password : %s\n", old, psw);
    rewind(fp);
    int flag;
    flag = fwrite(&utente_temp, sizeof(utente_t), 1, fp);
    if (!flag)
        return 0;
    flag = rename(oldbuff, buffer); //torna 0 se va tutto bene
    if (flag < 0)
        return 0;
    free(old);
    free(oldbuff);
    free(buffer);
    fclose(fp);
    return 1;
}

float visual_saldo(char *user)
{
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r");
    if (!fileuser)
        return 0;
    utente_t leggosaldo;
    fread(&leggosaldo, sizeof(utente_t), 1, fileuser);
    free(buffer);
    fclose(fileuser);
    return leggosaldo.saldo;
}

int visual_conto(char *user)
{
    int i;
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r");
    if (!fileuser)
        return 0;
    utente_t x;
    fread(&x, sizeof(utente_t), 1, fileuser);
    printf("\nNome : %s\nCognome : %s\nData di nascita : %s\nCitta' di nascita : %s\n\n", x.nome, x.cognome, x.data_di_nascita, x.city);
    printf("Carta di Credito : ");
    for (i = 0; i < 16; i++)
    {
        printf("%d", x.num_carta[i]);
        if ((i + 1) % 4 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
    printf("Intestatario : %s\nCVV : ", x.intestatario);
    for (i = 0; i < 3; i++)
        printf("%d", x.cvv[i]);
    printf("\nCodice carta : ");
    for (i = 0; i < 4; i++)
        printf("%d", x.codice_carta[i]);

    printf("\nsaldo : $ %.2f",x.saldo);
    printf("\n\n");
    free(buffer);
    fclose(fileuser);
    return 1;
}

char *data()
{
    int giorno, mese, anno;
    time_t now;
    now = time(NULL);
    struct tm *local = localtime(&now);
    giorno = local->tm_mday;
    mese = local->tm_mon + 1;
    anno = local->tm_year + 1900;
    char giorno_s[3], mese_s[3], anno_s[5];
    char *z;
    z = malloc((3 + strlen(giorno_s) + strlen(mese_s) + strlen(anno_s)) * sizeof(char));
    sprintf(giorno_s, "%02d", giorno);
    sprintf(mese_s, "%02d", mese);
    sprintf(anno_s, "%02d", anno);
    sprintf(z, "%s/%s/%s", giorno_s, mese_s, anno_s);
    return z;
}

char *ora()
{
    int ore, minuti, secondi;
    time_t now;
    now = time(NULL);
    struct tm *local = localtime(&now);
    ore = local->tm_hour;
    minuti = local->tm_min;
    secondi = local->tm_sec;
    char ore_s[3], minuti_s[3], secondi_s[3];
    char *z;
    z = malloc((3 + strlen(ore_s) + strlen(minuti_s) + strlen(secondi_s)) * sizeof(char));
    sprintf(ore_s, "%02d", ore);
    sprintf(minuti_s, "%02d", minuti);
    sprintf(secondi_s, "%02d", secondi);
    sprintf(z, "%s:%s:%s", ore_s, minuti_s, secondi_s);
    return z;
}

int vers(char *user, float importo)
{
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r+");
    if (!fileuser) return 0;
    utente_t leggosaldo;
    fread(&leggosaldo, sizeof(utente_t), 1, fileuser);
    leggosaldo.saldo += importo;
    leggosaldo.entrate+= importo;
    rewind(fileuser);
    fwrite(&leggosaldo, sizeof(utente_t), 1, fileuser);
    free(buffer);
    fclose(fileuser);
    return 1;
}

int bonifico(char *user, char *bonuser, float importo)
{
    if(!trovautente(bonuser,"user")) 
    {
        puts("User non esistente");
        return -1;
    }
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r+");
    if (!fileuser)
        return 0;
    utente_t leggosaldo;
    fread(&leggosaldo, sizeof(utente_t), 1, fileuser);
    leggosaldo.saldo -= importo;
    if (leggosaldo.saldo < 0)
    {
        puts("Non hai abbastanza soldi");
        free(buffer);
        fclose(fileuser);
        return 0;
    }
    free(buffer);
    buffer = malloc((1 + strlen(bonuser) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", bonuser, ".user");
    FILE *filebonuser = fopen(buffer,"r+");
    leggosaldo.uscite+=importo;
    utente_t x;
    fread(&x,sizeof(utente_t),1,filebonuser);
    x.saldo+=importo;
    int flag = vers(bonuser, importo);
    if(!flag) return 0;
    printf("Bonifico avvenuto con successo!\n");
    printf("\nSaldo precedente : $ %.2f\n\n",leggosaldo.saldo+importo);
    free(buffer);
    buffer = malloc((2 + strlen(bonuser) + strlen("entrate") * sizeof(char)));
    sprintf(buffer, "%s.%s", bonuser, "entrate");
    FILE *fileentrate = fopen(buffer,"r+");

    free(buffer);
    buffer = malloc((2 + strlen(bonuser) + strlen("uscite") * sizeof(char)));
    sprintf(buffer, "%s.%s", user, "uscite");
    FILE *fileuscite = fopen(buffer,"r+");

    free(buffer);
    buffer = malloc(200*sizeof(char));
    while(fgets(buffer, 200, fileentrate));
    fprintf(fileentrate,"\nL'user %s ti ha inviato un bonifico di $ %.2f in data %s alle ore %s\n",user,importo,data(),ora());

    free(buffer);
    buffer = malloc(200*sizeof(char));
    while(fgets(buffer, 200, fileuscite));
    fprintf(fileuscite,"Hai fatto un bonifico di $ %.2f a %s in data %s e ora %s\n", importo, bonuser, data(), ora());

    free(buffer);
    rewind(fileuser);
    fwrite(&leggosaldo, sizeof(utente_t), 1, fileuser);
    rewind(filebonuser);
    fwrite(&x,sizeof(utente_t),1,filebonuser);

    fclose(fileuser);
    fclose(fileentrate);
    fclose(fileuscite);
    fclose(filebonuser);
    return 1;
}

int trovautente(char *user, char *psw)
{
    char *buffer;
    lower(user);
    buffer = malloc((2 + strlen(user)+strlen(psw)) * sizeof(char));
    sprintf(buffer, "%s.%s", user, psw);
    FILE *file = fopen(buffer, "r");
    if (!file)
        return 0;
    fclose(file);
    free(buffer);
    return 1;
}

int prelievo(char *user,float importo)
{
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r+");
    if (!fileuser)
        return 0;
    utente_t leggosaldo;
    fread(&leggosaldo, sizeof(utente_t), 1, fileuser);
    leggosaldo.saldo -= importo;
    if (leggosaldo.saldo < 0)
    {
        printf("Non hai abbastanza soldi.\nIl tuo saldo attuale e' di $ %.2f\n", visual_saldo(user));
        free(buffer);
        fclose(fileuser);
        return 0;
    }
    printf("Il tuo saldo precedente era di $ %.2f\n",leggosaldo.saldo+importo);
    leggosaldo.uscite += importo;
    rewind(fileuser);
    fwrite(&leggosaldo, sizeof(utente_t), 1, fileuser);
    fflush(fileuser);
    printf("Il tuo saldo attuale e' di $ %.2f\n",visual_saldo(user));
    free(buffer);
    fclose(fileuser);
    return 1;

}

int visual_eus(char *user)
{
    char *buffer;
    buffer = malloc((1 + strlen(user) + strlen(".user") * sizeof(char)));
    sprintf(buffer, "%s%s", user, ".user");
    FILE *fileuser = fopen(buffer, "r+");
    if (!fileuser)
        return 0;
    utente_t leggo;
    fread(&leggo, sizeof(utente_t), 1, fileuser);
    printf("In questa sessione ci sono stati $ %.2f di entrate, $ %.2f di uscite",leggo.entrate,leggo.uscite);
    float netto;
    netto = leggo.entrate-leggo.uscite;
    if(netto<0) printf(", per una dimunuzione totale di $ %.2f\n", -1*netto);
    if(netto == 0) printf(", per cui, come si dice...Pari e patti\n");
    if(netto > 0) printf(", per un utile complessivo di $ %.2f.\n",netto);
    printf("Saldo iniziale : $ %.2f\nSaldo finale : $ %.2f\n",leggo.saldo-netto, leggo.saldo);
    leggo.entrate = 0;
    leggo.uscite = 0;
    rewind(fileuser);
    fwrite(&leggo,sizeof(utente_t),1,fileuser);
    free(buffer);
    fclose(fileuser);
    return 1;
}