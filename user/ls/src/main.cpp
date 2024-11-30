#include <stacsos/console.h>
#include <stacsos/directorydata.h>
#include <stacsos/list.h>
#include <stacsos/memops.h>
#include <stacsos/objects.h>
#include <stacsos/string.h>

using namespace stacsos;

int list_path(const char *path, bool longListing, bool recursive){
    
    // Verify path
    if (path == nullptr){
        console::get().write("error: usage: /usr/ls <path>\n");
        return 1;
    }

    // Get directory and verify it opened
    int d = object::open_directory(path);
    if (d == -1){
        console::get().writef("error: no path '%s'\n", path);
        return 1;
    }

    // Read directory
    directorydata dir_data;
    auto dir = object::read_directory((u64)d, &dir_data);
    // Keep reading, outputting and moving through directories
    while (dir != -1){
        // Write current file/directory to console
        if (longListing){
            // Long Listing
            if (dir_data.type == 1){
                // Directory
                console::get().writef("[D] %s\n", dir_data.dir_name);
            }
            else{
                // File
                console::get().writef("[F] %s       %d\n", dir_data.dir_name, dir_data.size);
            }
        }
        else{
            // No Long Listing
            console::get().writef("%s\n", dir_data.dir_name);
        }

        // Check if we need to list recursively
        if (dir_data.type == 1 && recursive){
            string *this_path = new string(path);
            string *current_dir_name = new string(dir_data.dir_name);
            string new_path;
            new_path = *this_path + "/" + *current_dir_name;
            list_path(new_path.c_str(), longListing, recursive);
        }

        memops::memset(dir_data.dir_name, 0, 255);
        // Advance dir
        dir = object::read_directory((u64)d, &dir_data);
    }

    return 0;
}

int main(const char *cmdline){

    // Parse arguments
    string *to_stringed = new string(cmdline);
    list<string> args = to_stringed->split(' ', true);

    const char *path = nullptr;

    bool opt_l = false; // Long listing option
    bool opt_r = false; // Recursive option

    for (string arg: args){
        const char *arg_chars = arg.c_str();
        if (arg_chars[0] == '-'){
            // Option argument
            for (int i = 1; i < arg.length(); i++){
                if (arg_chars[i] == 'l'){
                    opt_l = true;
                }
                else if (arg_chars[i] == 'r'){
                    opt_r = true;
                }
            }
        }
        else{
            // Directory argument
            path = arg_chars;
        }
    }

    // Start the recursive function with initial path and options
    return list_path(path, opt_l, opt_r);
}