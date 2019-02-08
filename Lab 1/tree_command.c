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

void bfs_tree(char *path[], int count, int level) {
	struct dirent *de;

	char *next_level[1024];
	int current = 0;

	for (int i = 0; i < count; i ++) {
		if (level == 0) {
			print_file(path[i], -1);
		}
		DIR *dr = opendir(path[i]);
		if (dr == NULL) {
			printf("Could not open the directory: %s\n", path[i]);
		}
		else {
			while ((de = readdir(dr)) != NULL) {
				if (de->d_name[0] == '.') {
					continue;
				}
				if (de->d_type == DT_DIR) {
					// store
					next_level[current] = (char *)malloc(sizeof(char)*255 + strlen(path[i]));
					strcpy(next_level[current], path[i]);
					strcat(next_level[current], "/");
					strcat(next_level[current], de->d_name);
					current ++;
				}
				else {
					char *disp;
					disp = (char *)malloc(sizeof(char) * (strlen(path[i]) + 1 + strlen(de->d_name)));
					strcat(disp, path[i]);
					strcat(disp, "/");
					strcat(disp, de->d_name);
					print_file(disp, level);
				}
			}
			closedir(dr);
		}
	}

	if (current != 0) {
		bfs_tree(next_level, current, level + 1);
	}
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
			char **paths;
			paths = (char **)malloc(1 * sizeof(char *));
			paths[0] = (char *)malloc(sizeof(char) * 255);
			strcpy(paths[0], ".");
			bfs_tree(paths, 1, 0);
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
			char **paths;
			paths = (char **)malloc(1 * sizeof(char *));
			paths[0] = (char *)malloc(sizeof(char) * 255);
			strcpy(paths[0], argv[2]);
			bfs_tree(paths, 1, 0);
		}
		else {
			printf("Usage of the program: ./tree [-d | -b] [path]\n");
		}
	}
	return 0;
}