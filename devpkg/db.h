#ifndef _db_h
#define _db_h

#define DB_FILE "/usr/local/.devpkg/db"
#define DB_DIR "/usr/local/.devpkg"

int DB_init(void);
int DB_list(void);
int DB_update(const char* url);
int DB_find(const char* url);

#endif

