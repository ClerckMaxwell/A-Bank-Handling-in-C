#include "bancomat.h"

#define size_of_user 30
#define size_of_psw 30

int main()
{
    char *buffer;
    char user[size_of_user], psw[size_of_psw], temp[5], scelta, bonuser[size_of_user];
    int end = 0;
    int i, fine, accesso = 0, nuovo_utente = 0, attempt = 0, cont, flag, guardia;
    printf("\tBANCA INTESA SAN RAFFAELE\nBenvenuto, hai gia' un conto nella nostra banca?\n");
    while (!end)
    {
        scanf("%s", temp);
        while ('\n' != getchar())
            ;
        lower(temp);
        if (strcmp(temp, "si") == 0)
        {
            printf("Bene! prova a fare l'accesso al tuo conto!\n\n");
            end = 2;
        }
        else if (strcmp(temp, "no") == 0)
        {
            printf("Bene allora crea un conto!\n");
            end = 1;
        }
        else
            printf("Purtoppo non ho capito, puoi riprovare?\n");
    } //while
    fine = 0;
    i = 0;
    guardia = 0;
    while (!fine)
    {
        attempt = 0;
        switch (end)
        {

        case 1:
            if (i == 0)
                puts("\nPer creare un conto ti basta creare un username e una password personale.\nUna volta che ti sarai registrato ti sara' chiesto di specificare le tue generalita', successivamente ti sara' assegnata un carta di credito personale.\nDigita qui l'username che vorresti:");

            scanf("%29s", user);
            while ('\n' != getchar())
                ;
            if (!trovautente(user, "user"))
            {
                buffer = malloc((2 + strlen(user) + strlen("user")) * sizeof(char));
                sprintf(buffer, "%s.%s", user,"user");
                FILE *file = fopen(buffer, "w");
                if (!file)
                {
                    puts("Impossibile creare l'user inserito, riprova");
                    i = 1;
                }
                else
                {
                    printf("L'username %s e' stato creato, ora inserisci una password!\n", user);
                    scanf("%29s", psw);
                    while ('\n' != getchar())
                        ;
                    free(buffer);
                    buffer = malloc((2 + strlen(user) + strlen(psw)) * sizeof(char));
                    sprintf(buffer, "%s.%s", user, psw);
                    FILE *filepsw = fopen(buffer, "w");
                    if (!filepsw)
                        puts("Impossibile creare questa password, riprova");
                    else
                    {
                        puts("Il tuo user e la tua password sono stati memorizzati con successo!\nIl tuo account e' stato creato e ora puoi effettuare l'accesso.");
                        end = 2;
                        nuovo_utente = 1;
                        free(buffer);
                        fclose(filepsw);
                    }
                }
            }
            else
            {
                printf("L'username %s e' gia' in uso, riprova con un altro username.\n", user);
                i = 1;
            }
            break;

        case 2:
            if (!guardia)
                puts("Inserisci qui il tuo username");

            scanf("%s", user);
            while ('\n' != getchar())
                ;
            if (!trovautente(user, "user"))
            {
                puts("User non esistente, riprova");
                guardia = 1;
                break;
            }
            else
            {
                puts("Inserire password");
                cont = 0;
                accesso = 0;
                while (!accesso)
                {
                    scanf("%s", psw);
                    while ('\n' != getchar())
                        ;
                    if (trovautente(user, psw))
                    {
                        puts("\nAccesso riuscito!\n");
                        if(!nuovo_utente)puts("Sarai reindirizzato al menu' principale\n");
                        usleep(500000);
                        fine = 1;
                        accesso = 1;
                    }
                    else
                    {
                        if (cont <= 3)
                        {
                            puts("Password errata riprova");
                            if (!nuovo_utente)
                                cont++;
                        }
                        if (cont >= 3 && !nuovo_utente)
                        {
                            attempt++;
                            if (attempt < 3)
                            {
                                flag = pass_recovery(user);
                                if (flag == 1)
                                {
                                    puts("Identita' verificata! Ora ti faro' reimpostare la tua password\nInserisci la tua nuova password:");
                                    scanf("%s", psw);
                                    while ('\n' != getchar())
                                        ;
                                    ;
                                    if (new_pass(user, psw))
                                    {
                                        puts("La tua password e' stata reimpostata con successo");
                                        accesso = 1;
                                        end = 2;
                                    }
                                    else
                                    {
                                        puts("Qualcosa e' andato gravemente storto");
                                        return EXIT_FAILURE;
                                    }
                                }
                                if (flag == 0)
                                {
                                    puts("Le informazioni che mi hai dato non corrispondono all'identita' dell'user. Non posso farti reimpostare la password.\nPuoi creare un account se vuoi, altrimenti continua a provare.\nInserire password:");
                                    cont = 0;
                                }
                                if (flag == -1)
                                {
                                    puts("Va bene continua a provare\nInserire password");
                                    cont = 0;
                                }
                            }
                            else
                            {
                                puts("Stai cercando di accedere troppe volte per cui devo bloccarti");
                                return EXIT_FAILURE;
                            }
                        }
                    }
                }
            }
            break;
        }
    } //fine accesso
    if (nuovo_utente)
    {
        createFileList(user);
        puts("Visto che sei un nuovo utente raccontami un po' di te! Questa azione e' necessaria ai fini della generazione della tua carta di credito e alla memorizzazione dei tuoi dati");
        flag = data_p_c(user, psw);
        if (!flag)
        {
            puts("Ops qualcosa e' andato storto");
            return EXIT_FAILURE;
        }
        puts("\nI tuoi dati sono stati memorizzati con successo!\n");
        puts("Genero la tua carta...\n");
        usleep(500000);
        flag = card_generator(user);
        puts("\nSarai reindirizzato verso il menu' principale\n");
        usleep(500000);
        if (!flag)
        {
            puts("Ops qualcosa e' andato storto");
            return EXIT_FAILURE;
        }
    } //menu
    fine = 0;
    printf("\nQuesto e' il menu' principale, puoi fare le seguenti cose :\n");
    printf("\n's' : SALDO\n'e' : LISTA ENTRATE\n'u' : LISTA USCITE\n'b' : BONIFICO\n'p' : PRELIEVO\n'v' : VERSAMENTO\n'i' : INFO CONTO\n'x' : ESCI\n\n");
    scanf("%c", &scelta);
    while ('\n' != getchar())
        ;
        List entrate = createList();
        List uscite = createList();
        l_m dati;
        l_m ops;
    while (!fine)
    {
        int guardia = 0;
        switch (scelta)
        {
        case SALDO:
            flag = printf("Il tuo saldo attuale e' di $ %.2f\n", visual_saldo(user));
            if (!flag)
                puts("Impossibile visualizzare il saldo");
            break;
        case INFO_CONTO:
        usleep(500000);
            flag = visual_conto(user);
            if (!flag)
                puts("Impossibile visualizzare le informazioni di questo user");
            break;
        case EXIT:
            fine = 1;
            guardia = 1;
            puts("Arrivederci, alla prossima!");
            break;

        case VERSAMENTO:
        usleep(500000);
            puts("Inserisci l'importo da versare");
            scanf("%f", &dati.importo);
            while('\n'!=getchar());
        if(dati.importo>0)
        {
            printf("\nSaldo precedente : $ %.2f\n\n",visual_saldo(user));
            flag = vers(user, dati.importo);
            if (!flag)
            {
                puts("Qualcosa e' andato storto");
                return EXIT_FAILURE;
            }
            printf("Versamento avvenuto con successo!\n");
            printf("Il tuo saldo attuale e' di $ %.2f\n", visual_saldo(user));
            buffer = malloc(12 * sizeof(char));
            strcpy(buffer, ora());
            dati.ora = malloc((1 + strlen(buffer)) * sizeof(char));
            strcpy(dati.ora, buffer);
            free(buffer);
            buffer = malloc(10 * sizeof(char));
            strcpy(buffer, data());
            dati.data = malloc((1 + strlen(buffer)) * sizeof(char));
            strcpy(dati.data, buffer);
            insertFirst(entrate, dati);
            printf("\n");
        }
        if(dati.importo==0) printf("Non puoi versare na somma nulla ahahahah\n");
        if(dati.importo < 0) printf("Quello che vuoi fare caro mio, si chiama prelievo\n");
            break;

        case LISTA_ENTRATE:
        printf("\nStorico entrate :\n");
            readFileEntrate(user);
        printf("\nIn questa sessione :\n");
            printEntrate(entrate);
            usleep(500000);
            break;

        case LISTA_USCITE:
        printf("\nStorico uscite :\n");
            readFileUscite(user);
        printf("\nIn questa sessione :\n");
            printUscite(uscite);
            usleep(500000);
            break;

        case BONIFICO:
            puts("A chi vuoi fare il bonifico?");
            scanf("%s", bonuser);
            while ('\n' != getchar())
                ;
            puts("Quanto vuoi inviare?");
            scanf("%f", &ops.importo);
            while('\n'!=getchar());
            if(ops.importo>0)
            {
            flag = bonifico(user, bonuser, ops.importo);
            if (flag<0)
            {
                puts("Il bonifico non e' andato a buon fine, user non esistente");
            }
            if(flag==0) puts("Il bonifico non e' andato a buon fine");
            if(flag == 1)
            {
                puts("Bonifico andato a buon fine");
                printf("Il tuo saldo attuale e' di $ %.2f\n",visual_saldo(user));
                
            }
            usleep(500000);
            }
            if(ops.importo ==0 ) printf("Non puoi fare bonifici nulli ahahaha\n");
            if(ops.importo<0) printf("Ehhh volevi imbrogliare\n");
            break;

        case PRELIEVO:
            puts("Quanto vuoi prelevare?");
            scanf("%f", &dati.importo);
            while('\n'!=getchar());
        if(dati.importo>0)
        {
            if(prelievo(user, dati.importo))
            {
            buffer = malloc(12 * sizeof(char));
            strcpy(buffer, ora());
            dati.ora = malloc((1 + strlen(buffer)) * sizeof(char));
            strcpy(dati.ora, buffer);
            free(buffer);
            buffer = malloc(10 * sizeof(char));
            strcpy(buffer, data());
            dati.data = malloc((1 + strlen(buffer)) * sizeof(char));
            strcpy(dati.data, buffer);
            insertFirst(uscite, dati);
            }
        usleep(500000);
        }
        if(dati.importo == 0) printf("Che vuoi che ti dia? Aria?\n");
        if(dati.importo < 0) printf("Quello che vuoi fare si chiama versamento");
            break;

            default:
            puts("Comando sconosciuto riprova");
            break;
        }
        if (!fine)
        {
            puts("\nPuoi fare altro se vuoi, altrimenti puoi uscire digitando 'x'");
                printf("\nTi ricordo che :\n's' : SALDO\n'e' : LISTA ENTRATE\n'u' : LISTA USCITE\n'b' : BONIFICO\n'p' : PRELIEVO\n'v' : VERSAMENTO\n'i' : INFO CONTO\n'x' : ESCI\n\n");
                scanf("%c", &scelta);
                while ('\n' != getchar());
        }
    }
        printListFileEntrate(entrate, user, dati);
        printListFileUscite(uscite, user, dati);
        printf("\n");
        visual_eus(user);
}