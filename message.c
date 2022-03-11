#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void freeDir(Dir* target);
void freeListDir(Dir* target){

	while(target){
		Dir* aux = target->next;
		freeDir(target);
		target = aux;
	}
}

void freeFile(File* target){
	free(target->name);
	free(target);
}

void freeListFile(File* target){

	while(target){
		File* aux = target->next;
		freeFile(target);
		target = aux;
	}
}


void freeDir(Dir* target){
	free(target->name);
	freeListDir(target->head_children_dirs);
	freeListFile(target->head_children_files);
	free(target);
}

void touch (Dir* parent, char* name) {

	int *ok = (int*)malloc(sizeof(int));
	*ok = 1;

	struct File *hcfs = parent->head_children_files;


	if(parent->head_children_files == NULL){
        struct File *newFile = (struct File*)malloc(sizeof(struct File));
        newFile->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
        strcpy(newFile->name, name);
        newFile->parent = parent;
        newFile->next = NULL;
		parent->head_children_files = newFile;
	}
	else{

		while(hcfs->next != NULL){
				if(strcmp(hcfs->name, name) == 0){
					*ok = 0;
					break;
				}
				hcfs = hcfs->next;
			}

		if(*ok && strcmp(hcfs->name, name)){
            struct File *newFile = (struct File*)malloc(sizeof(struct File));
        newFile->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
        strcpy(newFile->name, name);
        newFile->parent = parent;
        newFile->next = NULL;
			hcfs->next = newFile;
		}
		else{
			printf("File already exists\n");
		}

	}
	free(ok);
}

void mkdir (Dir* parent, char* name) {


	int *ok = (int*)malloc(sizeof(int));
	*ok = 1;

	struct Dir *hcds = parent->head_children_dirs;


	if(parent->head_children_dirs == NULL){
        struct Dir *newDir = (struct Dir*)malloc(sizeof(struct Dir));
        newDir->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
        strcpy(newDir->name, name);
        newDir->parent = parent;
        newDir->next = NULL;
        newDir->head_children_files = NULL;
        newDir->head_children_dirs = NULL;
		parent->head_children_dirs = newDir;
	}
	else{

		while(hcds->next != NULL){
				if(strcmp(hcds->name, name) == 0){
					*ok = 0;
					break;
				}
				hcds = hcds->next;
			}

		if(*ok && strcmp(hcds->name, name)){
            struct Dir *newDir = (struct Dir*)malloc(sizeof(struct Dir));
            newDir->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
            strcpy(newDir->name, name);
            newDir->parent = parent;
            newDir->next = NULL;
            newDir->head_children_files = NULL;
            newDir->head_children_dirs = NULL;
			hcds->next = newDir;
		}
		else{
			printf("Directory already exists\n");
		}

	}
	free(ok);
}

void ls (Dir* parent) {

	struct Dir *hcds;
	struct File *hcfs;
	hcds = parent->head_children_dirs;
	hcfs = parent->head_children_files;

	while(hcds != NULL){
		printf("%s\n", hcds->name);
		hcds = hcds->next;
	}
	while(hcfs != NULL){
		printf("%s\n", hcfs->name);
		hcfs = hcfs->next;
	}
	free(hcds);
	free(hcfs);
}

void rm (Dir* parent, char* name) {

	struct File *hcfs = parent->head_children_files;
	struct File *prev = hcfs;

	if(!hcfs){
		printf("Could not find the file\n");
		return;
	}
	else if(!hcfs->next){
		if(strcmp(hcfs->name, name) == 0){
			parent->head_children_files = NULL;
			freeFile(hcfs);
			return;
		}
		else{
			printf("Could not find the file\n");
			return;
		}
	}
	else if(hcfs->next && (strcmp(hcfs->name, name) == 0)){
		parent->head_children_files = hcfs->next;
		freeFile(hcfs);
		return;
	}
	while(hcfs->next){
		prev = hcfs;
		hcfs = hcfs->next;

		if ((strcmp(hcfs->name, name) == 0)){
			if(hcfs->next){
				prev->next = hcfs->next;
				freeFile(hcfs);
				return;
			}
			else{
				prev->next = NULL;
				freeFile(hcfs);
				return;
			}


		}
		else if((strcmp(hcfs->name, name) != 0)){
			if(!hcfs->next){
				printf("Could not find the file\n");
				break;
			}
		}

	}

}

void rmdir (Dir* parent, char* name) {

	struct Dir *hcds = parent->head_children_dirs;
	struct Dir *prev = hcds;

	if(!hcds){
		printf("Could not find the dir\n");
		return;
	}
	else if(!hcds->next){
		if(strcmp(hcds->name, name) == 0){
			freeDir(hcds);
			parent->head_children_dirs = NULL;
			return;
		}
		else{
			printf("Could not find the dir\n");
			return;
		}
	}
	else if(hcds->next && (strcmp(hcds->name, name) == 0)){
		parent->head_children_dirs = hcds->next;
		freeDir(hcds);
		return;
	}

	while(hcds->next){
		prev = hcds;
		hcds = hcds->next;
		if ((strcmp(hcds->name, name) == 0)){
			if(hcds->next){
				prev->next = hcds->next;
				freeDir(hcds);
				return;
			}
			else{
				prev->next = NULL;
				freeDir(hcds);
				return;
			}
		}
		else if((strcmp(hcds->name, name) != 0)){
			if(!hcds->next){
				printf("Could not find the dir\n");
				break;
			}
		}
	}
}

void cd(Dir** target, char *name) {

	struct Dir *hcds = (*target)->head_children_dirs;

	if(strcmp(name, "..") == 0){
		if((*target)->parent)
            (*target) = (*target)->parent;
        return;
	}

	if((*target)->head_children_dirs == NULL){
		printf("No directories found!\n");
		return;
	}

	while(hcds != NULL){
		if(strcmp(hcds->name, name) == 0){
			(*target) = hcds;
			return;
		}
		else if(hcds->next == NULL){
			printf("No directories found!\n");
			return;
		}
		else{
			hcds = hcds->next;
		}
	}
}

char *pwd (Dir* target) {

	char **s = (char**)malloc(MAX_INPUT_LINE_SIZE*sizeof(char));
	char *ret = (char*)malloc(MAX_INPUT_LINE_SIZE*sizeof(char));
	struct Dir *aux = target;
 	int *i = malloc(sizeof(int));
	*i = 0;
 	int *j = malloc(sizeof(int));

	while(aux){
		s[*i] = (char *)malloc(MAX_INPUT_LINE_SIZE*sizeof(char));
		strcpy(s[*i], aux->name);
		aux = aux->parent;
		(*i)++;
	}

	for((*j) = (*i)-1; (*j) >= 0; (*j)--) {
		strcat(ret, "/");
		strcat(ret, s[*j]);
		free(s[*j]);
	}

	free(s);
	free(i);
	free(j);

	return ret;
}

void stop (Dir* target) {
	freeDir(target);
}

void tree (Dir* target, int level) {

	if (target->head_children_dirs){
		printf("%*s", 4*level, "");
		printf("%s\n", target->head_children_dirs->name);
		tree(target->head_children_dirs, level + 1);

		if (target->head_children_files){
			printf("%*s", 4*level, "");
			printf("%s\n", target->head_children_files->name);
		}
	}

	if (target->next){
		printf("%*s", 4*(level-1), "");
		printf("%s\n", target->next->name);
		tree(target->next, level);
	}
}

void mv(Dir* parent, char *oldname, char *newname) {

	struct File* hcfs = parent->head_children_files;
	int okOldName = 0;
	int okNewName = 1;

	struct Dir* hcds = parent->head_children_dirs;
	struct Dir* ant = NULL;
	struct Dir* copieDir;
	int okOldNameD = 0;
	int okNewNameD = 1;

	while(hcfs){
		if(strcmp(hcfs->name, oldname) == 0){
			okOldName = 1;
			break;
		}
		hcfs = hcfs->next;
	}

	hcfs = parent->head_children_files;

	while(hcfs){
		if(strcmp(hcfs->name, newname) == 0){
			okNewName = 0;
			break;
		}
		hcfs = hcfs->next;
	}

	while(hcds){
		if(strcmp(hcds->name, newname) == 0){
			okNewNameD = 0;
		}
		hcds = hcds->next;
	}

	if(!okNewNameD){
		printf("File/Director already exists\n");
		return;
	}

	if(!okNewName){
		printf("File/Director already exists\n");
		return;
	}

	if(okOldName == 1 && okNewName == 1){
		rm(parent, oldname);
		touch(parent, newname);
		return;
	}

	hcds = parent->head_children_dirs;

	if(!hcds && !hcfs){
		printf("File/Director not found\n");
		return;
	}


	while(hcds){
		
		if(strcmp(hcds->name, oldname) == 0){
			okOldNameD = 1;
			copieDir = hcds;
		}
		hcds = hcds->next;
	}


	if(!okOldNameD && !okOldName){
		printf("File/Director not found\n");
		return;
	}

	hcds = parent->head_children_dirs;
	
	if(okOldNameD == 1 && okNewNameD == 1){
		strcpy(copieDir->name, newname);
		while(hcds->next)
			hcds = hcds->next;
		hcds -> next = copieDir;
		hcds = parent->head_children_dirs;
		while(hcds)
		{
			if(strcmp(hcds -> name , newname) == 0)
			{
				if(ant != NULL)
				{
					ant -> next = hcds -> next;
				}
				else
				{
					parent -> head_children_dirs = hcds -> next;
				}
				break;
			}
			ant = hcds;
			hcds = hcds -> next;
		}
		copieDir -> next = NULL;
	}
}

int main () {

	struct Dir *home = (struct Dir*)malloc(sizeof(struct Dir));
	home->name = (char*)malloc(5*sizeof(char));
	strcpy(home->name, "home");
	home->next = home->head_children_dirs = home->parent = NULL;
    home->head_children_files = NULL;

	struct Dir *curent = home;

    char *line = (char*)calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
    char *comand;
	do
	{
		fgets(line, MAX_INPUT_LINE_SIZE, stdin);
		comand = strtok(line, " ");

		if(!strcmp(comand, "touch")){

			comand = strtok(NULL, " ");
			comand = strtok(comand, "\n");
			touch(curent, comand);
		}

		if(!strcmp(comand, "mkdir")){

			comand = strtok(NULL, " ");
			comand = strtok(comand, "\n");
			mkdir(curent, comand);
		}

		if(!strcmp(comand, "rm")){

			comand = strtok(NULL, " ");
			comand = strtok(comand, "\n");
			rm(curent, comand);
		}

		if(!strcmp(comand, "rmdir")){

			comand = strtok(NULL, " ");
			comand = strtok(comand, "\n");
			rmdir(curent, comand);
		}

		if(!strcmp(comand, "cd")){

			comand = strtok(NULL, " ");
			comand = strtok(comand, "\n");
			cd(&curent, comand);
		}

		if(!strcmp(comand, "mv")){

			char *name = (char*)malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
			comand = strtok(NULL, " ");
			strcpy(name, comand);
			comand = strtok(NULL, " ");
			comand = strtok(comand, "\n");
			mv(curent, name, comand);
			free(name);
		}

		if(!strcmp(comand, "pwd\n")){
            char* ret = pwd(curent);
			printf("%s\n", ret);
            free(ret);
		}

		if(!strcmp(comand, "ls\n")){
			ls(curent);
		}

		if(!strcmp(comand, "tree\n")){
			tree(curent, 0);
		}

		if(!strcmp(line, "stop\n")){
			free(line);
			free(comand);
			break;
		}

	} while (1);
	stop(home);

	return 0;
}
