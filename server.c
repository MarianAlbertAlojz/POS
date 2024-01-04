//
// Created by PC1 on 04/01/2024.
//

#include "server.h"
/*int server(int argc, char *argv[]){
    srand(time(NULL));
    int sockfd;
    struct sockaddr_in serv_addr;

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }
    listen(sockfd, 5);
    printf("------------------------------------------------------\n");
    printf("Server sa  spustil, nastavte parametre hry.\n");
    printf("------------------------------------------------------\n\n");

    while (maxPocetHracov < 2 || maxPocetHracov > 3){
        printf("*** Vyberte z maximalneho poctu hracov (2-3) ***\n");
        scanf("%d",&maxPocetHracov);
        if (maxPocetHracov < 2 || maxPocetHracov > 3){
            printf("Zadali ste nespravnu volbu, zadajte znova !\n");
        }
    }

    if (maxPocetHracov == 2){
        while (velkostMapy < 3 || velkostMapy > 9){
            printf("*** Vyberte z rozmedzia velkosti mapy, minimalne (3), maximalne (9) ***\n");
            scanf("%d",&velkostMapy);
            if (velkostMapy < 3 || velkostMapy > 9){
                printf("Zadali ste nespravnu volbu, zadajte znova !\n");
            }
        }
    } else{
        while (velkostMapy < 5 || velkostMapy > 9){
            printf("*** Vyberte z rozmedzia velkosti mapy, minimalne (5), maximalne (9) ***\n");
            scanf("%d",&velkostMapy);
            if (velkostMapy < 5 || velkostMapy > 9){
                printf("Zadali ste nespravnu volbu, zadajte znova !\n");
            }
        }
    }

    zacinajuci = rand() % maxPocetHracov;
    char **mapa = malloc(velkostMapy * sizeof(char *));
    for (int i = 0; i < velkostMapy; ++i) {
        mapa[i] = malloc(velkostMapy* sizeof(char));
        for(int j=0; j<velkostMapy; j++){
            mapa[i][j] = '-' ;
        }
    }

    printf("------------------------------------------------------\n");
    printf("Hra bola uspesne nastavena s parametrami:\npocet hracov %d\nvelkost mapy:%dx%d\n",maxPocetHracov,velkostMapy,velkostMapy);
    printf("------------------------------------------------------\n\n");

    pthread_t hraci[maxPocetHracov], kontrola;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    pthread_barrier_t cakajHracov;
    pthread_barrier_init(&cakajHracov,NULL,maxPocetHracov);

    SERVERD servD = {0, maxPocetHracov, 0, menoVyhercu, 0, zacinajuci, mapa, velkostMapy, infoVyhry, &cakajHracov, &mutex};
    HRACD * hraciD = malloc(maxPocetHracov * sizeof(HRACD));

    printf("Caka sa na %d hracov...\n",servD.maximalnyPocHracov);

    // Pthread 'create' / 'join'
    for (int i = 0; i < maxPocetHracov; i++) {
        hraciD[i].idHraca = i;
        hraciD[i].sockfd = sockfd;
        hraciD[i].infoHraca = infoHraca;
        hraciD[i].infoHry = infoHry;
        hraciD[i].okInfo = okInfo;
        hraciD[i].spoluD = &servD;
        pthread_create(&hraci[i], NULL, hraciFun, &hraciD[i]);
    }


    for (int i = 0; i < maxPocetHracov; i++) {
        pthread_join(hraci[i], NULL);
    }





    printf("------------------------------------------------------\n");
    printf("Server Skoncil\n");
    printf("------------------------------------------------------\n\n");
    close(sockfd);

    return 0;
}*/