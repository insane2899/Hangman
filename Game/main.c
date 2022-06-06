#include "WordList.h"
#include "WordPartitions.h"
#include<stdio.h>
#include<string.h>

void mask(char* word, char* guesses, char* output);
int contains_unguessed(char* guess);
int mycmp(char* current_hint,char* key);
int already_guessed(char* guessed_letters,char guess);

int already_guessed(char* guessed_letters,char guess){
	for(int i=0;i<strlen(guessed_letters);i++){
		if(*(guessed_letters+i)==guess){
			return 1;
		}
	}
	return 0;
}

int mycmp(char* current_hint,char* key){
	for(int i=0;i<strlen(current_hint);i++){
		if(*(current_hint+i)!=*(key+i)){
			return 1;
		}
	}
	return 0;
}

void mask(char* word,char* guesses,char* output){
	for(int i=0;i<strlen(word);i++){
		int flag = 0;
		for(int j=0;j<strlen(guesses);j++){
			if(*(word+i) == *(guesses+j)){
				*(output+i) = *(word+i);
				flag = 1;
				break;
			}
		}
		if(!flag){
			*(output+i) = '_';
		}
	}
}

int contains_unguessed(char* guess){
	for(int i=0;i<strlen(guess);i++){
		if(*(guess+i)=='_'){
			return 1;
		}
	}
	return 0;
}

void main(int argc, char** argv){

	if(argc==2){
		printf("\nWhat word length do you want? (<1 or non-numeric initiates self-tests) ");
		size_t length = 0;
		scanf("%ld",&length);
		WordList list = wordlist_from_file(*(argv+1),length);
		//printf("\nThere are %ld words of length %ld",list.size,length);
		int incorrect_guesses = 5;
		char* current_hint = (char*)malloc(sizeof(char)*length);
		char* guessed_letters = (char*)malloc(sizeof(char)*length+1);
		*guessed_letters = '\0';
		for(int i=0;i<length;i++){
			*(current_hint+i)='_';
		}
		int guessed_letters_count = 0;
		while(1){
			printf("\n---------------------------------");
			char* temp = (char*)malloc(sizeof(char)*length);
			printf("\nThere are %ld words still possible.",list.size);
			printf("\nYou have %d incorrect guesses left",incorrect_guesses);
			printf("\nCurrent hint: %s",current_hint);
			printf("\nGuessed letters: %s",guessed_letters);
			printf("\nWhat letter do you want to guess? ");
			char guess = ' ';
			scanf("\n");
			scanf("%c",&guess);
			printf("You have guessed \'%c\'",guess);
			while(already_guessed(guessed_letters,guess)){
				printf("\nYou have already guessed this letter. Guess another letter");
				scanf("\n");
				scanf("%c",&guess);
			}
			*(guessed_letters+guessed_letters_count)=guess;
			guessed_letters_count++;
			*(guessed_letters+guessed_letters_count)='\0';
			WordListPartitions* partitions = partitions_init();
			char* output = (char*)malloc(sizeof(char)*length);
			for(size_t i=0;i<list.size;i++){
				strcpy(temp,list.data+i*(length+1));
				mask(temp,guessed_letters,output);
				partitions_insert(partitions,output,temp);
			}
			char** key = (char**)malloc(sizeof(char*)*1);
			key[0] = (char*)malloc(sizeof(char)*length);
			WordList** list2 = (WordList**)malloc(sizeof(WordList*));
			partitions_max_partition(partitions,key,list2);
			int cmp = mycmp(current_hint,*key);
			strcpy(current_hint,*key);
			list = **list2;
			if(!cmp){
				printf("Unfortunately \'%c\' is not in the word",guess);
				incorrect_guesses--;
			}
			else{
				printf("You have guessed correctly");
			}
			if(!contains_unguessed(current_hint)){
				printf("\nYou win! The word was \'%s\'\n",current_hint);
				break;
			}
			if(incorrect_guesses==0){
				printf("\nYou have run out of incorrect guesses. You lose!\n");
				break;
			}
		}
	}
	else if(argc==3){
		printf("\nWhat word length do you want? (<1 or non-numeric initiates self-tests) ");
		size_t length = 0;
		scanf("%ld",&length);
		WordList list = wordlist_from_file(*(argv+1),length);
		printf("\nThere are %ld words of length %ld",list.size,length);
		int incorrect_guesses = 5;
		char* current_hint = (char*)malloc(sizeof(char)*length);
		char* guessed_letters = (char*)malloc(sizeof(char)*length+1);
		*guessed_letters = '\0';
		for(int i=0;i<length;i++){
			*(current_hint+i)='_';
		}
		int guessed_letters_count = 0;
		while(1){
			printf("\n---------------------------------");
			printf("\nThe possible words are: ");
			char* temp = (char*)malloc(sizeof(char)*length);
			for(size_t i=0;i<list.size;i++){
				strcpy(temp, list.data + i*(length+1));
				printf("%s ",temp);
			}
			printf("\nThere are %ld words still possible.",list.size);
			printf("\nYou have %d incorrect guesses left",incorrect_guesses);
			printf("\nCurrent hint: %s",current_hint);
			printf("\nGuessed letters: %s",guessed_letters);
			printf("\nWhat letter do you want to guess? ");
			char guess = ' ';
			scanf("\n");
			scanf("%c",&guess);
			printf("You have guessed \'%c\'",guess);
			while(already_guessed(guessed_letters,guess)){
				printf("\nYou have already guessed this letter. Guess another letter ");
				scanf("\n");
				scanf("%c",&guess);
			}
			*(guessed_letters+guessed_letters_count)=guess;
			guessed_letters_count++;
			*(guessed_letters+guessed_letters_count)='\0';
			printf("\nThe partite sets from your guess are:\n");
			WordListPartitions* partitions = partitions_init();
			char* output = (char*)malloc(sizeof(char)*length);
			for(size_t i=0;i<list.size;i++){
				strcpy(temp,list.data+i*(length+1));
				mask(temp,guessed_letters,output);
				partitions_insert(partitions,output,temp);
			}
			partitions_print(partitions);
			char** key = (char**)malloc(sizeof(char*)*1);
			key[0] = (char*)malloc(sizeof(char)*length);
			WordList** list2 = (WordList**)malloc(sizeof(WordList*));
			partitions_max_partition(partitions,key,list2);
			int cmp = mycmp(current_hint,*key);
			strcpy(current_hint,*key);
			list = **list2;
			if(!cmp){
				printf("Unfortunately \'%c\' is not in the word",guess);
				incorrect_guesses--;
			}
			else{
				printf("You have guessed correctly");
			}
			if(!contains_unguessed(current_hint)){
				printf("\nYou win! The word was \'%s\'\n",current_hint);
				break;
			}
			if(incorrect_guesses==0){
				printf("\nYou have run out of incorrect guesses. You lose!\n");
				break;
			}
		}
	}
	else{
		printf("Invalid Input\n");
	}

}