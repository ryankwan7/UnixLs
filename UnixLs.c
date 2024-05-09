#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

void print_file_info(char *path, int i_opt, int l_opt)
{
    DIR *dir;
    struct dirent *dp;
    
    if ((dir = opendir(path)) == NULL)
    {
        perror(path);
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        struct stat buf;
        stat(dp->d_name, &buf);
        lstat(dp->d_name, &buf);

        // skip . and ..
        if ((dp->d_name)[0] == '.')
            continue;

        // print the inode number
        if (i_opt)
            printf("%ld ", buf.st_ino);

        if (l_opt)
        {
            // check if file is a symbolic link
            if (S_ISLNK(buf.st_mode))
                printf("l");
            // check if file is a directory
            else if (S_ISDIR(buf.st_mode))
                printf("d");
            // check if file is a regular file
            else
                printf("-");

            printf((buf.st_mode & S_IRUSR) ? "r" : "-"); // check if file is readable for user
            printf((buf.st_mode & S_IWUSR) ? "w" : "-"); // check if file is writable for user
            printf((buf.st_mode & S_IXUSR) ? "x" : "-"); // check if file is executable for user
            printf((buf.st_mode & S_IRGRP) ? "r" : "-"); // check if file is readable for group
            printf((buf.st_mode & S_IWGRP) ? "w" : "-"); // check if file is writable for group
            printf((buf.st_mode & S_IXGRP) ? "x" : "-"); // check if file is executable for group
            printf((buf.st_mode & S_IROTH) ? "r" : "-"); // check if file is readable for others
            printf((buf.st_mode & S_IWOTH) ? "w" : "-"); // check if file is writable for others
            printf((buf.st_mode & S_IXOTH) ? "x" : "-"); // check if file is executable for others
            printf(" ");

            // check number of hard links to the file
            printf("%-4ld ", buf.st_nlink);

            // get user name
            struct passwd *pw;
            pw = getpwuid(buf.st_uid);
            printf("%-8s ", pw->pw_name);

            // get group name
            struct group *grp;
            grp = getgrgid(buf.st_gid);
            printf("%-8s ", grp->gr_name);

            // get file size
            printf("%-8ld ", buf.st_size);

            // get modification time
            struct tm *tm;
            tm = localtime(&buf.st_mtime);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%b %d %Y %H:%M", tm);
            printf("%s ", time_str);
        }

        // get file name
        printf("%s", dp->d_name);

        // check if file is a symbolic link
        if (l_opt && S_ISLNK(buf.st_mode))
        {
            char link_target[4096];
            char full_path[4096];

            snprintf(full_path, sizeof(full_path), "%s/%s", path, dp->d_name);

            ssize_t len = readlink(full_path, link_target, sizeof(link_target) - 1);
            if (len != -1)
            {
                link_target[len] = '\0';
                printf(" -> %s", link_target);
            }
        }

        printf("\n");
    }

    // close the directory
    closedir(dir);
}

int main(int argc, char *argv[])
{
    int i_opt = 0;
    int l_opt = 0;

    int dir_start = 0;
    int opt_start = 0;
    int num_dir = 0;
    // check if -i, -l, -il or -li are given
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if(opt_start == 0)
                opt_start = i;

            if (strcmp(argv[i], "-i") == 0)
                i_opt = 1;
            else if (strcmp(argv[i], "-l") == 0)
                l_opt = 1;
            else if (strcmp(argv[i], "-il") == 0 || strcmp(argv[i], "-li") == 0)
            {
                i_opt = 1;
                l_opt = 1;
            }
        }
        else
        {
            num_dir++;
            if(dir_start == 0)
                dir_start = i;
        }
    }

    // check if options precede directories
    if (num_dir != 0 && dir_start < opt_start)
    {
        printf("Error: Options must precede directories.\n");
        return 1;
    }

    if (num_dir > 0)
    {
        for (int i = dir_start; i < argc; i++)
        {
            printf("%s:\n", argv[i]);
            print_file_info(argv[i], i_opt, l_opt);
            printf("\n");
        }
    }
    // No directories specified
    else if (num_dir == 0)
    {
        print_file_info(".", i_opt, l_opt);
        printf("\n");
    }

    return 0;
}
