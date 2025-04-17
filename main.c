#include <stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

const int MAX_LINE_LENGTH = 100;
const int MAX_CLIENTS=100;
enum Level {
    SILVER,
    GOLD,
    PLATINUM,
  };
//Defining our structures
struct DATE {
    int day;
    int month;
    int year;
};

struct Loyalty_Program_Details {
    bool Loyalty_Membership;
    int Membership_level;
    long total_points;
};
struct Transaction_Details {
    struct DATE dateT;
    char T_id[100];
    float total_amount;
    float DiscountT;
    int loyalty_Points_Earned;
};

struct Client {
    struct DATE dateC;
    long id;
    char nom[100];
    char prenom[100];
    int numtel;
    char adr[100];
    struct Loyalty_Program_Details LPD;
    struct Transaction_Details TD;
};


//Declaring a chained list
struct Node {
    struct Client data;
    struct Node* next;
};

struct Node *head = NULL;

// Creat nod
struct Node* createNode(struct Client data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Échec de l'allocation mémoire.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertAtFront(struct Client data) {
    struct Node *newNode = createNode(data);
    if (newNode) {
        newNode->next = head;
        head = newNode;
    }
}

void deleteNode(long id) {
    struct Node *temp = head, *prev = NULL;
    //if the nod doesn't exist
    if (temp != NULL && temp->data.id == id) {
        head = temp->next;
        free(temp);
        printf("Client avec ID %ld supprimé.\n", id);
        return;
    }
    // Looking dor the nod to delete
    if (temp != NULL) {
        while (temp->next != NULL && temp->data.id != id) {
            prev = temp;
            temp = temp->next;
        }
        //Deleting a client
        prev->next = temp->next;
        free(temp);
        printf("Client avec ID %ld supprimé.\n", id);
    }else{
        // If the client doesn't exist
        printf("Client avec ID %ld introuvable.\n", id);
        return;
    }
}

// Displaying all the records
void displayClients() {
    struct Node *temp = head;
    while (temp != NULL) {
        printf("ID: %ld\n Nom: %s\n Prénom:%s\n Téléphone : %d\n Adresse:%s\n",
               temp->data.id, temp->data.nom, temp->data.prenom, temp->data.numtel, temp->data.adr);
        temp = temp->next;
    }
}
// Adding a client
void ADD(struct Client *client) {
    printf("Date (jj mm aaaa): ");
    scanf("%d %d %d\n", &client->dateC.day, &client->dateC.month, &client->dateC.year);
    printf("ID : ");
    scanf("%ld\n", &client->id);
    printf("Nom : ");
    scanf("%s\n", client->nom);
    printf("Prénom : ");
    scanf("%s\n", client->prenom);
    printf("Téléphone : ");
    scanf("%d\n", &client->numtel);
    printf("Adresse : ");
    scanf("%s\n", client->adr);

    printf("Montant : ");
    scanf("%f\n",&client->TD.total_amount);
    printf("Discount : ");
    scanf("%f\n",&client->TD.DiscountT);

    client->LPD.Loyalty_Membership = true; // Exemple de valeur par défaut
    client->LPD.Membership_level = SILVER;
    client->LPD.total_points = 0;
    insertAtFront(*client);

    FILE *file = fopen("clients_data.txt", "a");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier pour l'ajout.\n");
        return;
    }
    fprintf(file, "%d#%d#%d#%ld#%s#%s#%d#%s\n",
        client->dateC.day,client->dateC.month, client->dateC.year,
        client->id, client->nom, client->prenom,
        client->numtel, client->adr);

    fclose(file);

    printf("Client ajouté avec succès et enregistré dans le fichier.\n");
}
void Check_Membership_level(  long int idc , long LP ) {
    //find client based on iid
    struct Node *temp = head;
    while (temp != NULL) {
        if (temp->data.id == idc) {
            // check loyatly points based on reference
            //Modify level if needed
        }
    }
}
void modifyLoyaltyPoints(long id, long newPoints) {
    struct Node *temp = head;
    // Traverse the list to find the client
    while (temp != NULL) {
        if (temp->data.id == id) {
            // Update the loyalty points
            temp->data.LPD.total_points += newPoints;
            printf("Loyalty points updated for client ID %ld.\n", id);
            return;
        }
        temp = temp->next;
    }
    // If the client is not found
    printf("Client with ID %ld not found.\n", id);

}
void findClientOfTheYear(int year) {
    struct Node *temp = head;
    struct Client *bestClient = NULL;
    long maxPoints = 0;
    // Traverse the list
    while (temp != NULL) {
        // Check if the client's loyalty program is active and points are from the given year
        if (temp->data.LPD.Loyalty_Membership && temp->data.dateC.year == year) {
            if (temp->data.LPD.total_points > maxPoints) {
                maxPoints = temp->data.LPD.total_points;
                bestClient = &temp->data;
            }
        }
        temp = temp->next;
    }
    // Display the client with the highest points
    if (bestClient != NULL) {
        printf("\n--- Client of the Year ---\n");
        printf("Name: %s %s\n", bestClient->nom, bestClient->prenom);
        printf("ID: %ld\n", bestClient->id);
        printf("Total Loyalty Points: %ld\n", bestClient->LPD.total_points);
        printf("Membership Level: %s\n",
               bestClient->LPD.Membership_level == SILVER ? "SILVER" :
               bestClient->LPD.Membership_level == GOLD ? "GOLD" : "PLATINUM");
    } else {
        printf("No clients found for the year %d.\n", year);
    }
}
void Add_Transaction_Details(long idc) {
    struct Node *temp = head;
    int test = 0;

    while (temp != NULL) {
        if (temp->data.id == idc) {
            struct Transaction_Details *td = &temp->data.TD;
            printf("Enter la date du transaction: ");
            scanf("%d %d %d", &td->dateT.day, &td->dateT.month, &td->dateT.year);
            printf("Enter le montant total: ");
            scanf("%f", &td->total_amount);
            printf("Vous avez un discount ? (Oui:1 / Non:0): ");
            scanf("%d", &test);
            if (test == 1) {
                printf("Enter votre discount : ");
                scanf("%f", &td->DiscountT);
            } else {
                td->DiscountT = 0;
            }
            // Calculate loyalty points earned
            float effectiveAmount = td->total_amount * (1 - td->DiscountT);
            td->loyalty_Points_Earned = (int)(effectiveAmount * 0.03);
            // Update client's loyalty points
            temp->data.LPD.total_points += td->loyalty_Points_Earned;

            // Check and update membership level
            if (temp->data.LPD.total_points > 1000) {
                temp->data.LPD.Membership_level = PLATINUM;
            } else if (temp->data.LPD.total_points > 500) {
                temp->data.LPD.Membership_level = GOLD;
            } else {
                temp->data.LPD.Membership_level = SILVER;
            }

            printf("\n--- Détails de la transaction ajoutée ---\n");
            printf("Date de Transaction : %02d-%02d-%d\n", td->dateT.day, td->dateT.month, td->dateT.year);
            printf("Montant : %.2f\n", td->total_amount);
            printf("Discount : %.2f\n", td->DiscountT);  // Fixed printf
            printf("Points de fidélité : %d\n", td->loyalty_Points_Earned);
            printf("Le total des points de fidélité: %ld\n", temp->data.LPD.total_points);
            printf("Niveau d'adhésion: %s\n",
                temp->data.LPD.Membership_level == SILVER ? "SILVER" :
                temp->data.LPD.Membership_level == GOLD ? "GOLD" : "PLATINUM");

            FILE *file = fopen("transaction_data_multiple_clients.txt", "a");
            if (file == NULL) {
                printf("Erreur d'ouverture du fichier pour l'ajout.\n");
                return;
            }
            fprintf(file, "%ld#%02d-%02d-%d#%s#%.2f#%.2f#%d\n",
                    temp->data.id,
                    td->dateT.day, td->dateT.month, td->dateT.year,
                    td->T_id, td->total_amount, td->DiscountT, td->loyalty_Points_Earned);
            fclose(file);
            return;
        }
        temp = temp->next;
    }
    printf("Client avec l'ID %ld est introuvable.\n", idc);
}


void Modify(struct Client *c) {
    int choix;
    printf("1. Modifier le nom\n");
    printf("2. Modifier le prénom\n");
    printf("3. Modifier le numéro de téléphone\n");
    printf("4. Modifier l'adresse\n");
    printf("5. Modifier les points de fidélité\n");
    printf("6. Ajouter des achats\n");
    printf("Entrez votre choix : ");
    if (scanf("%d", &choix) != 1) {
        printf("Erreur de saisie.\n");
        return;
    }
    switch (choix) {
        case 1:
            printf("Entrez le nouveau nom : ");
        scanf("%s", c->nom);  // Correction pour passer par pointeur
        break;
        case 2:
            printf("Entrez le nouveau prénom : ");
        scanf("%s", c->prenom);
        break;
        case 3:
            printf("Entrez le nouveau numéro de téléphone : ");
            scanf("%d", &c->numtel);
        break;
        case 4:
            printf("Entrez la nouvelle adresse : ");
            scanf("%s", c->adr);
        break;
        case 5: {
            long newLP;
            printf("Entrez les nouveaux points de fidélité : ");
            if (scanf("%ld", &newLP) == 1) {
                modifyLoyaltyPoints(c->id, newLP);
            } else {
                printf("Erreur de saisie.\n");
            }
            break;
        }
        case 6: {
            Add_Transaction_Details(c->id);
            break;
        }
        default:
            printf("Choix invalide.\n");
        break;
    }
}

// Function to check if a year is a leap year
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to calculate the difference in days between two dates
int daysBetween(struct DATE d1, struct DATE d2) {
    static const int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int days1 = d1.year * 365 + d1.day;
    for (int i = 0; i < d1.month - 1; i++) {
        days1 += monthDays[i];
    }
    days1 += (d1.year / 4 - d1.year / 100 + d1.year / 400); // Add leap years

    int days2 = d2.year * 365 + d2.day;
    for (int i = 0; i < d2.month - 1; i++) {
        days2 += monthDays[i];
    }
    days2 += (d2.year / 4 - d2.year / 100 + d2.year / 400); // Add leap years
    int r=days2 - days1;
    return r;

}

// Function to update the client data file
void updateClientsFile() {
    FILE *file = fopen("C:/Users/Pc/CLionProjects/PROJETC/clients_data.txt", "w");
    if (!file) {
        perror("Error opening file for update");
        return;
    }

    struct Node *temp = head;
    while (temp != NULL) {
        fprintf(file, "%d#%d#%d#%ld#%s#%s#%d#%s\n",
                temp->data.dateC.day, temp->data.dateC.month, temp->data.dateC.year,
                temp->data.id, temp->data.nom, temp->data.prenom,
                temp->data.numtel, temp->data.adr);
        temp = temp->next;
    }
    fclose(file);
}

// Function to delete transactions of a client from the transactions file
void deleteTransactionsFromFile(long clientId) {
    FILE *file = fopen("C:/Users/Pc/CLionProjects/PROJETC/transaction_data_multiple_clients.txt", "r");
    FILE *tempFile = fopen("C:/Users/Pc/CLionProjects/PROJETC/temp_transactions.txt", "w");
    if (!file || !tempFile) {
        perror("Error opening files for deleting transactions");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        long id;
        sscanf(line, "%ld#", &id);
        if (id != clientId) {
            fputs(line, tempFile); // Copy lines that don't match the client ID
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original transaction file with the updated one
    remove("C:/Users/Pc/CLionProjects/PROJETC/transaction_data_multiple_clients.txt");
    rename("C:/Users/Pc/CLionProjects/PROJETC/temp_transactions.txt", "C:/Users/Pc/CLionProjects/PROJETC/transaction_data_multiple_clients.txt");
}
void Control_Accounts() {
    time_t t = time(NULL);
    struct tm current_time = *localtime(&t);

    struct DATE today = {
        .day = current_time.tm_mday,
        .month = current_time.tm_mon + 1,
        .year = current_time.tm_year + 1900
    };

    // Open the transaction file once
    FILE *file = fopen("C:/Users/Pc/CLionProjects/PROJETC/transaction_data_multiple_clients.txt", "r");
    if (!file) {
        perror("Error opening transaction file");
        return;
    }

    struct Node *temp = head;
    struct Node *prev = NULL;

    while (temp != NULL) {
        struct DATE mostRecentTransaction = {0, 0, 0};
        rewind(file);  // Reset file pointer for reading transactions again

        // Parse the transaction file to find the most recent transaction for this client
        char line[256];
        long clientId;
        int day, month, year;
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%ld#%d-%d-%d", &clientId, &day, &month, &year);
            if (clientId == temp->data.id) {
                struct DATE transactionDate = {day, month, year};

                // Compare dates to find the most recent transaction
                if (mostRecentTransaction.year == 0 || daysBetween(mostRecentTransaction, transactionDate) < 0) {
                    mostRecentTransaction = transactionDate;
                }
            }
        }

        // Check inactivity based on the most recent transaction
        if (mostRecentTransaction.year == 0 || daysBetween(mostRecentTransaction, today) > 365) {
            printf("Deleting client with ID %ld (Name: %s %s) - Last Purchase: %02d/%02d/%d\n",
                   temp->data.id, temp->data.nom, temp->data.prenom,
                   mostRecentTransaction.day, mostRecentTransaction.month, mostRecentTransaction.year);

            // Delete associated transactions
            deleteTransactionsFromFile(temp->data.id);

            // Remove client from the linked list
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }

            struct Node *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        } else {
            prev = temp;
            temp = temp->next;
        }
    }

    fclose(file);  // Close the file after processing all clients

    // Update the client data file after deletions
    updateClientsFile();
    printf("Client file updated.\n");
}



void parse_line(char *line, struct Client *client) {
    sscanf(line, "%d#%d#%d#%ld#%[^#]#%[^#]#%d#%[^\n]",
           &client->dateC.day,
           &client->dateC.month,
           &client->dateC.year,
           &client->id,
           client->nom,
           client->prenom,
           &client->numtel,
           client->adr);
}
int main( ) {
    int choix;
    struct Client client;
    char line[256];
    // Open the file
    FILE *file = fopen("C:/Users/Pc/CLionProjects/PROJETC/clients_data.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Read each line and parse it
    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &client);
        insertAtFront(client);
    }
    fclose(file);

    do {
        printf("\n--- MENU ---\n");
        printf("1. Ajouter un client\n");
        printf("2. Afficher les clients\n");
        printf("3. Modifier le compte client\n");
        printf("4. Supprimer un client\n");
        printf("5. Controle des compte clients\n");
        printf("6. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ADD(&client);
                printf("Client ajouté avec succès.\n");
            break;
            case 2:
                displayClients();
            break;
            case 3:
                printf("Entrer l'ID du client à traité : ");
            scanf("%ld", &client.id);
            Modify (&client);
            break;

            case 4:
                printf("Entrez l'ID du client à supprimer : ");
                long id;
                scanf("%ld", &id);
                deleteNode(id);
                updateClientsFile();
                deleteTransactionsFromFile(id);
            break;
            case 5:
                Control_Accounts();
            printf("Comptes inactifs depuis plus d'un an supprimés.\n");
            default :
                printf("Au revoir !\n");
            break;
        }
    } while (choix != 5);

    return 0;
}

