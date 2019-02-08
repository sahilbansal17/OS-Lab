#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

void dfs_tree(char *path, char *parent) {
	struct dirent *de;

	DIR *dr = opendir(path);

	if (dr == NULL) {
		printf("Could not open the directory: %s\n", path);
	}
	else {
		while ((de = readdir(dr)) != NULL) {
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
				continue;
			}
			if (de->d_type == DT_DIR) {
				char *new_path = (char *)malloc(sizeof(char)*255);
				strcat(new_path, de->d_name);
				strcat(new_path, "/");
				printf("%s\n", new_path);
			}
			else {
				printf("%s\n", de->d_name);
			}
		}
	}
	closedir(dr);
}

int main () {

	dfs_tree(".", ".");
	return 0;
}