#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct card{
    char color; 
    int n;
};

void shuffle(struct card *array, size_t n)
{
    if (n > 1){
        for (size_t i = 0; i < n - 1; i++){
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            struct card t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void printCard(struct card c){
    if(c.color == 'w'){
        if(c.n == 13) printf("WILD");
        else if(c.n == 14) printf("WILD+4");
    } else {
        if(c.n <= 9) printf("%c %d", c.color, c.n);
        else if(c.n == 10) printf("%c SKIP", c.color);
        else if(c.n == 12) printf("%c +2", c.color);
    }
}

// 색 입력받기
char askColor(){
    printf("색 선택 (r/g/b/y): ");
    char c;
    scanf(" %c", &c);
    return c;
}

int main()
{
    srand(time(NULL));

    int deckTop = 0; // 스택 자료형 
    int pCount = 0, oCount = 0;

    struct card cards[56];   // 색 카드 36 + 능력 카드 20
    struct card pHand[50];
    struct card oHand[50];
    struct card pile;

    int id = 0;
    char cols[4] = {'r','g','b','y'};

    for(int i = 0; i < 4; i++){
        for(int j = 1; j <= 9; j++){           
            cards[id++] = (struct card){cols[i], j};
        }
        cards[id++] = (struct card){cols[i],10};
        cards[id++] = (struct card){cols[i],12};
    }


    for(int i = 0; i < 4; i++) 
		cards[id++] = (struct card){'w',13};
    for(int i = 0; i < 4; i++) 
		cards[id++] = (struct card){'w',14};

    shuffle(cards, id);

    // 처음에 카드 7장 뽑아야함
    for(int i = 0; i < 7; i++){
        pHand[pCount++] = cards[deckTop++];
        oHand[oCount++] = cards[deckTop++];
    }

    pile = cards[deckTop++];

    int turn = 0;

    while(true){
        printf("\n======================================\n");
        printf("현재 버림패: ");
        printCard(pile);
        printf("\n");
		// 봇 턴
        if(turn % 2 == 0){
            printf("\n~~~~~~~~~~ 상대 턴 ~~~~~~~~~~\n");

            bool played = false;
            int i;

            for(i = 0; i < oCount; i++){
                if( oHand[i].color == pile.color || 
                    oHand[i].n == pile.n || 
                    oHand[i].color == 'w' ){
                    played = true;
                    break;
                }
            }

            if(!played){
                printf("1장 뽑음\n");
                oHand[oCount++] = cards[deckTop++];
            } 
            else {
                pile = oHand[i];
                printf("카드를 냄 : ");
                printCard(pile);
                printf("\n");

                for(int j=i; j<oCount-1; j++) oHand[j] = oHand[j+1];
                oCount--;

                // WILD 색 선택
                if(pile.color == 'w'){
                    char cc = cols[rand()%4];
                    pile.color = cc;
                    printf("상대가 색을 %c 로 변경!\n", cc);
                }

                // 능력 카드 처리
                if(pile.n == 10){ // SKIP
                    printf("플레이어 턴 스킵!\n");
                    turn++;
                }
                else if(pile.n == 12){ // +2
                    printf("플레이어가 2장 드로우!\n");
                    pHand[pCount++] = cards[deckTop++];
                    pHand[pCount++] = cards[deckTop++];
                }
                else if(pile.n == 14){ // wild +4
                    printf("플레이어가 4장 드로우!\n");
                    for(int k=0;k<4;k++)
                        pHand[pCount++] = cards[deckTop++];
                }
            }

            if(oCount == 0){
                printf("\n상대 승리!\n");
                break;
            }
        }

        // 플레이어 턴
        else{
            printf("\n~~~~~~~~~~ 플레이어 턴 ~~~~~~~~~~\n");
            printf("당신의 패 (%d장):\n", pCount);
            for(int i = 0; i < pCount; i++){
                printf("%d: ", i);
                printCard(pHand[i]);
                printf("\n");
            }

            int choice;
            printf("낼 카드 id 선택 (-1 = 드로우): ");
            scanf("%d", &choice);

            if(choice == -1){
                pHand[pCount++] = cards[deckTop++];
                printf("1장 뽑음\n");
            }
            else if(choice >= 0 && choice < pCount){
                struct card c = pHand[choice];

                // 낼 수 없으면 드로우
                if(!(c.color == pile.color || c.n == pile.n || c.color=='w')){
                    printf("너 룰 모르지\n");
                    pHand[pCount++] = cards[deckTop++];
                }
                else{
                    pile = c;
                    printf("카드를 냄: ");
                    printCard(pile);
                    printf("\n");

                    for(int j=choice; j<pCount-1; j++)
                        pHand[j] = pHand[j+1];
                    pCount--;

                    // wild 색 선택
                    if(pile.color == 'w'){
                        pile.color = askColor();
                    }

                    if(pile.n == 10){ // skip
                        printf("상대 턴 스킵!\n");
                        turn++;
                    }
                    else if(pile.n == 12){ // +2
                        printf("상대가 2장 드로우!\n");
                        oHand[oCount++] = cards[deckTop++];
                        oHand[oCount++] = cards[deckTop++];
                    }
                    else if(pile.n == 14){ // +4
                        printf("상대가 4장 드로우!\n");
                        for(int k=0;k<4;k++)
                            oHand[oCount++] = cards[deckTop++];
                    }
                }
            }

            if(pCount == 0){
                printf("\n플레이어 승리!\n");
                break;
            }
        }

        turn++;
    }

    return 0;
}
