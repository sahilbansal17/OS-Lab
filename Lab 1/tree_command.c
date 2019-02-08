#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

void print_file (char *name, int level) {
	for (int i = 0; i < level; i ++) {
		printf("|   ");
	}
	if (level != -1)
		printf("|-- ");
	printf("%s\n", name);
}

void dfs_tree(char *path, int level) {
	struct dirent *de;

	DIR *dr = opendir(path);

	if (dr == NULL) {
		printf("Could not open the directory: %s\n", path);
	}
	else {
		if (level == 0)
			print_file(path, -1);
		while ((de = readdir(dr)) != NULL) {
			if (de->d_name[0] == '.') {
				continue;
			}
			if (de->d_type == DT_DIR) {
				char *new_path = (char *)malloc(sizeof(char)*255 + strlen(path));
				strcpy(new_path, path);
				strcat(new_path, "/");
				strcat(new_path, de->d_name);
				print_file(de->d_name, level);
				dfs_tree(new_path, level + 1);
			}
			else {
				print_file(de->d_name, level);
			}
		}
	}
	closedir(dr);
}

void bfs_tree(char *path, int level) {

}

int main (int argc, char *argv[]) {

	if (argc == 1) {
		// by default dfs
		dfs_tree(".", 0);
	}
	if (argc == 2) {
		// whether bfs or dfs
		if (strcmp(argv[1], "-d") == 0) {
			dfs_tree(".", 0);
		}
		else if (strcmp(argv[1], "-b") == 0) {
			bfs_tree(".", 0);
		}
		else {
			printf("Usage of the program: ./tree [-d | -b] [path]\n");
		}
	}
	if (argc == 3) {
		// whether bfs or dfs as well as path given
		if (strcmp(argv[1], "-d") == 0) {
			dfs_tree(argv[2], 0);
		}
		else if (strcmp(argv[1], "-b") == 0) {
			bfs_tree(argv[2], 0);
		}
		else {
			printf("Usage of the program: ./tree [-d | -b] [path]\n");
		}
	}
	return 0;
}