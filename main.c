#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct LCRSnode *folderptr;
typedef folderptr leftsibling, rightsibling, leftchild, parent;

typedef struct newfile {
    char* file_name;
	char* file_cont;
	int file_auth;
	// char* file_time;
	char* file_loc;
	char viewtype;
	char file_level;
	struct tm* t;
	struct newfile* rightsibling;
}file;

typedef struct LCRSnode
{
	char *name;
	char *time;
    int auth;
	folderptr parent, leftchild, rightsibling, leftsibling;

    int file_count;
    char viewtype;
	struct tm* t;
	struct newfile* in;
}folder;

folderptr present;

typedef struct LCRStree
{
    folder* headernode;
    folder* currentnode;
}foldertree;

typedef struct stack
{
char *inputs[1000];
int top;
}stack;


int is_empty(stack* stk)
{
    if(stk->top == -1)
        return 1;

    else
        return 0;
}

void push(stack* stk, char* s)
{
    stk->top += 1;
    stk->inputs[stk->top] = s;
}

void pop(stack* stk)
{
    if(is_empty(stk) == 1)
        return;
    else
        stk->top -= 1;
}

stack* make_stack()
{
    stack* stk = (stack*)malloc(sizeof(stack));
    stk->top = -1;
    return stk;
}

file* makefile(char* filename)
{
    time_t timer;
    timer = time(NULL);
    file* tmp = (file*)malloc(sizeof(file));
    if (tmp != NULL)
    {
        tmp->file_auth = 755;
        strncpy(tmp->file_name, filename, 20);
        tmp->rightsibling = NULL;

        if(filename[0] == '.')
        {
            tmp->viewtype = 'u';
        }
        else
        {
            tmp->viewtypw = 's';
        }

        tmp->t= localtime(&timer);
        return tmp;
        }
    }
}

folder* root_directory()
{
    time_t timer;
    timer = time(NULL);
    folder* rootdir = (folder*)malloc(sizeof(folder));
    if(rootdir != NULL)
    {
        rootdir->leftchild = NULL;
        rootdir->rightsibling = NULL;
        rootdir->parent = NULL;
        strncpy(rootdir->name, "/", 20);
        rootdir->auth = 755;
        rootdir->viewtype = 's';
        rootdir->t = localtime(&timer);
        return rootdir;
    }
}

foldertree* make_tree(folder* rootdir)
{
    foldertree* tmp = (foldertree*)malloc(sizeof(foldertree));
    if(tmp != NULL)
    {
        tmp->headernode = rootdir;
        tmp->currentnode = rootdir;
        return tmp;
    }
}

folder* delete_directory(foldertree* ftree, char* fname)
{
	folder* tmp = NULL;
	tmp = ftree->currentnode->leftchild;
	while (tmp != NULL)
	{
		if (strcmp(tmp->name, fname) == 0)
		{
			break;
		}
		tmp = tmp->Rightsibling;
	}
	return tmp;
}

foldertree* ltree;
stack* pstack;

int main()
{
    folder* root = root_directory();
    ltree = make_tree(root);
    pstack = make_stack();

    char command[50];
    char input[50];

    while(1)
    {

    }
}


/*void cd_(char *path) // cd
{
    if(chdir(path) == -1)
    {
        printf("Directory does not exist.\n");
    }
}
*/

/*void mkdir_(char *dname) // mkdir
{
    if(mkdir(dname)!=0)
    {
        printf("Can't make the directory\n");
    }
}*/

void pwd_(foldertree *ftree)
{
    stack* dirstack;
    folder* current = ftree->currentnode;
    if (current->parent == NULL)
    {
        printf("/\n");
        return;
    }
    while(current->parent != NULL)
    {
        push(dirstack, current->name);
        current = current->parent;
    }
    while(1)
    {
        if(is_empty(dirstack) == 1)
            break;
        else
        {
            printf("/%s", dirstack->inputs[dirstack->top]);
            pop(dirstack);
        }
    }
    printf("\n");

}

void rm_(foldertree* ftree, char* fname, int opt) // rm
{
    folder* folder_del;
    if (opt == 0)
    {
        if (rm_file(ftree, fname, opt) == 0)
		{
			folder_del = delete_directory(ftree, fname);
			if (folder_del != NULL)
				printf("can't remove the directory\n");
		}

	}
	else if (opt == 1)
	{
		if (rm_file(ftree, fname, opt) == 0)
		{
			folder_del = delete_directory(ftree, fname);
			if (folder_del != NULL)
				printf("can't remove the directory\n");
		}

	}
	else if (opt == 2)
	{
		if (rm_file(ftree, fname, opt) == 0)
		{
			folder_del = delete_directory(ftree, fname);
			if (folder_del != NULL)
            {
                rm_dir(ltree, fname, opt);
            }
		}
	}
	else if (opt == 3)
	{
		if (rm_file(ftree, fname, opt) == 0)
		{
			folder_del = delete_directory(ftree, fname);
			if (folder_del != NULL)
            {
                rm_dir(ltree, fname, opt);
            }
		}
	}
}

int rm_file(foldertree* dtree, char* fname, int opt)
{
    file* file_del = NULL;
    flie* file_find = NULL;
    file* file_prev = NULL;
    int first_child;
    file_find = file_prev = ftree->currentnode->in;

    if(file_prev == NULL)
    {
        printf("error\n");
        return 0;
    }

    else if(strcmp(file_find->name, fname) == 0)
    {
        ftree->currentnode->in = file_find->rightsibling;
        file_del = file_find;
        first_child = 1;
    }

    else
    {
        file_find = file_find->rightsibling;
        while(file_find != NULL)
        {
            if(strcmp(file_find->name, fname) == 0)
            {
                file_del = file_find;
                break;
            }
            else
            {
                file_prev = file_find;
                file_find = file_find->rightsibling;
            }
        }
    }
    if(file_del != NULL)
    {
        file_prev->rightsibling = file_del->rightsibling;
        free(file_del);
        return 1;
    }
    else
    {
        return 0;
    }

}

int rm_dir(foldertree* ftree, char* fname, int opt)
{
    folder* folder_del = NULL;
    folder* folder_find = NULL;
    folder* folder_prev = NULL;
    int first_child;

    folder_find = folder_prev = ftree->currentnode->leftchild;

    if(folder_prev == NULL)
    {
        printf("error\n");
        return 0;
    }

    else if(strcmp(folder_find->name, fname) == 0)
    {
        ftree->currentnode->leftchild = folder_find->rightsibling;
        folder_del = folder_find;
        first_child = 1;
    }

    else
    {
        folder_find = folder_find->rightsibling;
        while(folder_find != NULL)
        {
            if(strcmp(folder_find->name, fname) == 0)
            {
                folder_del = folder_find;
                break;
            }
            else
            {
                folder_prev = folder_find;
                folder_find = folder_find->rightsibling;
            }
        }
    }
    if(folder_del != NULL)
    {
        if(folder_del->leftchild != NULL && opt != 3) // 하위 디렉토리가 있
        {
            printf("can't remove : not empty\n");
            return 0;
        }
        if(first_child== 1)
        {
            rm_subdir(folder_del->leftchild);
            free(folder_del);
        }

        else
        {
            folder_prev->rightsibling = folder_del->rightsibling;
            rm_subdir(folder_del->leftchild);
            free(folder_del);
        }
    }
    else
    {
        pritnf("that directory does not exist.\n");
    }
}

void rm_subdir(folder* subdir)
{
    if(subdir != NULL)
    {
        rm_subdir(subdir->leftchild);
        rm_subdir(subdir->rightsibling);
        free(subdir);
    }
}


/*void cat_(int argc, char* argv[]) // cat
{
    char temp[MAX_SIZE];
    int rownum = 0;

    if(strcmp(argv[1], "-n") == 0)
    {
        FILE *fp = fopen(argv[2], "r");

        if (fp == NULL)
        {
            printf("can't open\n");
        }
        else
        {
            while(fgets(temp, MAX_SIZE, fp))
            {
                rownum++;
                printf("%d : ", rownum);
                fputs(temp, fp);
            }
            fclose(fp);
        }
    }

    else
    {
        FILE *fp = fopen(argv[2], "r");

        if (fp == NULL)
        {
            printf("can't open\n");
        }
        else
        {
            while(fgets(temp, MAX_SIZE, fp))
            {
                fputs(temp, fp);
            }
            fclose(fp);
        }
    }
}
*/


/*void cat_write()
{
    char temp[MAX_SIZE];

}*/



void cp_(char* argv[]) //cp
{
    char temp;

    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "w");

    if(f1 == NULL)
    {
        printf("file error\n");
        return;
    }

    while(!feof(f1))
    {
        temp = (char)fgetc(f1);
        fputc((int)temp, f2);
    }
    fclose(f1);
    fclose(f2);
}

//void find_();


void ls_(foldertree* ftree, int opt)
{
    folder* present = ftree->currentnode;
    folder* tmp;
    folder* childs[MAX_SIZE];
    int i = 0;

    if(present->leftchild = NULL)
    {
        printf("\n");
    }
    else
    {
        tmp = present->leftchild->rightsibling;
        childs[i] = present->leftchild;
        i++;
        while(tmp != NULL)
        {
            childs[i] = tmp;
            i++;
            tmp = tmp->rightsibling;
        }
    }


    if(opt == 0) // 옵션 X
    {
        int cnt;
        for(cnt = 0; cnt<i; cnt++)
        {
            if(childs[cnt]->viewtype == 's') //디렉토리
            {
                printf("%s ", childs[cnt]->name);
            }
        }

        if(present->in != NULL) //파일
        {
            if(present->in->viewtype == 's')
            {
                printf("%s  ", present->in->file_name);
            }
            while(present->in->rightsibling != NULL)
            {
                present->in = present->in->rightsibling;

                if(present->in->viewtype == 's')
                {
                    printf("%s  ", present->in->file_name);
                }
            }
        }

    }

    else if(opt == 1) // -l (파일 속성 표시)
    {
        int cnt;
        for(cnt =0; cnt < i; cnt++)
        {
            if(childs[cnt]->viewtype = 's')
            {
                printf("d");
                authtostr(childs[cnt]->auth);
                printf("  %02d %02d %02d:%02d", childs[cnt]->t->tm_mon+1, childs[cnt]->t->tm_mday, childs[cnt]->t->tm_hour, childs[cnt]->t->tm_min);
                printf("  %s\n", childs[cnt]->name);
            }
        }
        if(present->in != NULL)
        {
            if(present->in->viewtype == 's')
            {
                printf("-");
                authtostr(present->in->file_auth);
                printf("  %02d %02d %02d:%02d", present->in->t->tm_mon+1, present->in->t->tm_mday, present->in->t->tm_hour, present->in->t->tm_min);
                printf("  %s\n", present->in->file_name);
            }

            while(present->in->rightsibling != NULL)
            {
                present->in = present->in->rightsibling;
                if(present->in->viewtype == 's')
                {
                    printf("-");
                    authtostr(present->in->file_auth);
                    printf("")
                }
            }
        }
    }

    else if(opt == 2)// 옵션 -a (모든 파일 표시)
    {
        int cnt;
        for(cnt = 0; cnt<i; cnt++)
        {
            printf("%s ", childs[cnt]->name);
        }

        if(present->in != NULL) //파일
        {
            printf("%s  ", present->in->file_name);

            while(present->in->rightsibling != NULL)
            {
                present->in = present->in->rightsibling;
                printf("%s  ", present->in->file_name);
            }
        }
    }

    else //옵션 -al
    {
        int cnt;
        for(cnt =0; cnt < i; cnt++)
        {
            printf("d");
            authtostr(childs[cnt]->auth);
            printf("  %02d %02d %02d:%02d", childs[cnt]->t->tm_mon+1, childs[cnt]->t->tm_mday, childs[cnt]->t->tm_hour, childs[cnt]->t->tm_min);
            printf("  %s\n", childs[cnt]->name);
        }
        if(present->in != NULL)
        {
            printf("-");
            authtostr(present->in->file_auth);
            printf("  %02d %02d %02d:%02d", present->in->t->tm_mon+1, present->in->t->tm_mday, present->in->t->tm_hour, present->in->t->tm_min);
            printf("  %s\n", present->in->file_name);

            while(present->in->rightsibling != NULL)
            {
                present->in = present->in->rightsibling;

                printf("-");
                authtostr(present->in->file_auth);
                printf("")
            }
        }
    }
}


/*void get_time(tm *t)
{
    printf("%02d %02d %02d:%02d", t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);

}*/

void authtostr(int auth)
{
    char tmp[4];
	int i;

	sprintf(tmp, "%d", auth);

    for(i=0; i<3; i++)
    {
        switch (tmp[i])
		{
		case 0:
			printf("---");
			break;
		case 1:
			printf("--x");
			break;
		case 2:
			printf("-w-");
			break;
		case 3:
			printf("-wx");
			break;
		case 4:
			printf("r--");
			break;
		case 5:
			printf("r-x");
			break;
		case 6:
			printf("rw-");
			break;
		case 7:
			printf("rwx");
			break;
		}
    }

}
