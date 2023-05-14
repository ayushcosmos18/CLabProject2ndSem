#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct DATA{
    char id[10];
    char name[50];
    char money[50];
    char code[5];
};

char* encrypt(char string[50],int cipher)
{
    for(int i = 0; (i < 100 && string[i] != '\0'); i++)
    {
        string[i] = string[i] + cipher; 
    }
    return string;

}


char* decrypt(char string[50],int cipher)
{
    for(int i = 0; (i < 100 && string[i] != '\0'); i++)
    {
        string[i] = string[i] - cipher; 
    }
    return string;

}

int sum(int num)
{
    int sum=0;
   while(num!=0) 
   {
        int rem=num%10;
        sum+=rem;
        num=num/10;
   }
   return sum;
}

int main()
{
    FILE *file;
    file= fopen("Datasheet.csv","r");
    if(file==NULL)
    {
        printf("Something went wrong");
        return 1;
    }
    struct DATA data[100];
    int read=0;
    int records=0;
    do
    {
        read = fscanf(file,
                            "%9[^,],%49[^,],%49[^,],%4[^,]\n", 
                            data[records].id,
                            data[records].name,
                            data[records].money,
                            data[records].code);
                            
        if(read==4)
            {records++;}
        if(read!=4 && !feof(file))
        {
            printf("%d",read);
            printf("File format wrong");
            return 1;
         }

        if(ferror(file))
        {
            printf("Error reading records");
            return 1;
        }

    } while (!feof(file));

    fclose(file);
    
    int cipher;

    printf("\tWELCOME\n\n");
    printf("Enter your ATM CARD NUMBER\n");
    char card[10];
    gets(card);
    int check=0;
    int customer_index;
    for(int i=0;i<=records;i++)
    {
        if(strcmp(card,data[i].id)==0)
        {
            customer_index=i;
            check=1;
            break;
        }
    }
    if(check==0)
    {
        printf("Would you like to make an account with us? (Y/N)\n");
        char newacc;
        scanf("%c",&newacc);
        if(newacc=='Y')
        {
            strcpy(data[records+1].id,card);
            printf("Enter your PIN\n");
            scanf("%s",data[records+1].code);
            cipher = atoi(data[records+1].code); cipher=sum(cipher); 
            encrypt(data[records+1].code,cipher);
            printf("Enter your name\n");getchar();
            gets(data[records+1].name);
            encrypt(data[records+1].name,cipher);
            printf("Enter money to be deposited in the account\n");
            gets(data[records+1].money);
            encrypt(data[records+1].money,cipher);

            file= fopen("Datasheet.csv","a");

            if(file==NULL)
                {
                    printf("Something went wrong");
                    return 1;
                }
            fprintf(file,
                            "%s,%s,%s,%s\n", 
                            data[records+1].id,
                            data[records+1].name,
                            data[records+1].money,
                            data[records+1].code);
            printf("\n\n\tNEW ACCOUNT ADDED");
                printf("\n*********************\n");

            if(feof(file))
            {
                printf("New Account Added");
                fclose(file);
                return 0;
            }
        }
    }

    if(check==1)
    {
        printf("Enter your 4 digit PIN\n");
        char KEY[5];
        gets(KEY);
        decrypt(data[customer_index].code,sum(atoi(KEY)));
        if(strcmp(data[customer_index].code,KEY)!=0)
        {
            printf("\nWRONG PIN");
                printf("\n*********************\n");
            return 1;
        }
        else
        {
            printf("Welcome %s\n", decrypt(data[customer_index].name,sum(atoi(KEY))));
            printf("Choose\n");
            printf("1 for WITHDRAW\n");
            printf("2 for DEPOSIT\n");
            printf("3 for VIEW BALANCE\n");
            printf("4 for CHANGE PIN\n");
            printf("---------------------\n");
            int choiceinatm;
            scanf("%d",&choiceinatm);decrypt(data[customer_index].money,sum(atoi(KEY)));
            if(choiceinatm==3)
            {
                printf("YOUR BALANCE IS :\n");
                printf("\t Rs. %s",data[customer_index].money);
                printf("\n\n\tTHANK YOU");
                printf("\n*********************\n");

            }
            else if(choiceinatm==2||choiceinatm==1||choiceinatm==4)
            {
                file=fopen("Datasheet.csv","w");
                if(choiceinatm==2)
                {
                    printf("What is the amount you want to deposit? \n");
                    int deposit;
                    scanf("%d",&deposit);
                    for(int i=0;i<records;i++)
                    {
                        if(i!=customer_index)
                        {
                            fprintf(file, "%s,%s,%s,%s\n"
                                                        ,data[i].id
                                                        ,data[i].name
                                                        ,data[i].money
                                                        ,data[i].code);
                        }
                        else{
                            int temp=atoi(data[i].money);
                            temp=temp+deposit;
                            sprintf(data[i].money,"%d",temp);

                            encrypt(data[i].code,sum(atoi(KEY)));
                            encrypt(data[i].name,sum(atoi(KEY)));
                            encrypt(data[i].money,sum(atoi(KEY)));

                            fprintf(file, "%s,%s,%s,%s\n"
                                                        ,data[i].id
                                                        ,data[i].name
                                                        ,data[i].money
                                                        ,data[i].code);        
                            printf("\n\n\tDEPOSIT SUCCESSFUL");
                            printf("\n*********************\n");                    
                        }
                    }

                }
                else if(choiceinatm==1)
                {
                    printf("What is the amount to be withdrawn?\n");
                    int withdrawal;
                    scanf("%d",&withdrawal);
                    int tempmoney=atoi(data[customer_index].money);

                    if(tempmoney>=withdrawal)
                    {
                        for(int i=0;i<records;i++)
                        {
                        if(i!=customer_index)
                        {
                            fprintf(file, "%s,%s,%s,%s\n"
                                                        ,data[i].id
                                                        ,data[i].name
                                                        ,data[i].money
                                                        ,data[i].code);
                        }
                        else{
                            tempmoney=tempmoney-withdrawal;
                            sprintf(data[i].money,"%d",tempmoney);

                            encrypt(data[i].code,sum(atoi(KEY)));
                            encrypt(data[i].name,sum(atoi(KEY)));
                            encrypt(data[i].money,sum(atoi(KEY)));

                            fprintf(file, "%s,%s,%s,%s\n"
                                                        ,data[i].id
                                                        ,data[i].name
                                                        ,data[i].money
                                                        ,data[i].code); 
                            printf("\n\n\tWITHDRAWAL SUCCESSFUL");
                printf("\n*********************\n");                           
                        }
                        }
                    }
                    else
                    {
                        printf("INSUFFICIENT BALANCE\n");
                printf("\n*********************\n");

                        for(int i=0;i<records;i++)
                        {
                            if(customer_index!=i)
                            {
                            fprintf(file, "%s,%s,%s,%s\n"
                                                        ,data[i].id
                                                        ,data[i].name
                                                        ,data[i].money
                                                        ,data[i].code);
                            }
                            else{
                                encrypt(data[i].code,sum(atoi(KEY)));
                            encrypt(data[i].name,sum(atoi(KEY)));
                            encrypt(data[i].money,sum(atoi(KEY)));

                            fprintf(file, "%s,%s,%s,%s\n"
                                                        ,data[i].id
                                                        ,data[i].name
                                                        ,data[i].money
                                                        ,data[i].code); 
                            }
                        }
                    }
                }else if(choiceinatm==4)
                {
                    printf("Enter your new PIN: \n");
                    scanf("%s",data[customer_index].code);

                    int KEY2=sum(atoi(data[customer_index].code));

                    for(int i=0;i<records;i++)
                    {
                        if(i!=customer_index)
                        {
                            fprintf(file, "%s,%s,%s,%s\n"
                                                   ,data[i].id
                                                   ,data[i].name
                                                   ,data[i].money
                                                   ,data[i].code);
                        }
                        else
                        {   
                            fprintf(file, "%s,%s,%s,%s\n"
                                                   ,data[i].id
                                                   ,encrypt(data[i].name,KEY2)
                                                   ,encrypt(data[i].money,KEY2)
                                                   ,encrypt(data[i].code,KEY2));
                            printf("\n\n\tPIN CHANGED");
                printf("\n*********************\n");
                        }
                    }
                }
                fclose(file);
            }
        }

    }

}