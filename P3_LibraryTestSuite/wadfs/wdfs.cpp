//
// Created by Shashank Gutta on 12/1/24.
//
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <filesystem>
#include "../libWad/Wad.h"

static int do_getattr(const char *path, struct stat *st) {
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if ( strcmp( path, "/" ) == 0 )
    {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    }
    else
    {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = ((Wad*)fuse_get_context()->private_data)->getSize(path);
    }
    return 0;
}

static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    if (strcmp( path, "/" ) == 0 ) {
        vector<string> directory;
        ((Wad*)fuse_get_context()->private_data)->getDirectory(path, &directory);
        for (int i = 0; i < directory.size(); i++) {
            filler(buffer, directory[i].c_str(), NULL, 0);
        }

    }
    return 0;

}

static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    return ((Wad*)fuse_get_context()->private_data)->getContents(path, buffer, size, offset);
}

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,

};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Not enough arguments." << endl;
        exit(EXIT_SUCCESS);
    }

    string wadPath = argv[argc-2];

    if (wadPath.at(0) != '/') {
        wadPath = string(get_current_dir_name()) + "/" + wadPath;
    }
    Wad* myWad = Wad::loadWad(wadPath);

    argv[argc - 2] = argv[argc - 1];
    argc--;

    //fuse_get_context()->private_datta
    return fuse_main(argc, argv, &operations, myWad);
}
