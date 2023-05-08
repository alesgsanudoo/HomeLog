/* 
 * A really simple Log-in, log-out system
 * with mysql connection.
 *
 * This class will contain everything
 * related to mysql connection and user creating.
 *
 * Created by Alejandro S, 05/06/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <mysql.h>
#include <unistd.h>

#define USER_EXISTS (0);
#define CREATED_FILE (0);
#define CLOSING_CONN (0);
#define USER_AVAIL (1);
#define ERROR (-1);
#define SUCCESS (0);

void show_information(MYSQL *con);
void insert_information(MYSQL *con, char *query);
void delete_information(MYSQL *con, char *query);
int check_information(MYSQL *con, char *name);

static char host[200] = {0};
static char user[200] = {0};
static char pass[200] = {0};
static char db[200] = {0};
static char *socket = NULL;
unsigned int port = 3306;
unsigned int flags = 0;

/* 
 * The main will create the database and tables in 
 * order to make the program work correctly. 
 */

int main(int argc, char *argv[]) {

	FILE *data_ptr = NULL;
	printf("#######################\n");
	if (access("database.txt", F_OK) != 0) {
	  data_ptr = fopen("database.txt", "w");
	  if (data_ptr == NULL) {
	  	printf("Something went wrong creating the file!\n");
	  	return ERROR;
	  }
	  printf("Creating database file...\n");
	  printf("Make sure to modify it before running the code again.\n");
	  fprintf(data_ptr, "host: localhost\n");
	  fprintf(data_ptr, "user: root\n");
	  fprintf(data_ptr, "password: 1234\n");
	  fprintf(data_ptr, "database: test\n");
	  fprintf(data_ptr, "port: 3306\n");
	  printf("#######################\n");
	  return CREATED_FILE;
	} 
	else {
		data_ptr = fopen("database.txt", "r");
	  if (data_ptr == NULL) {
	  	printf("Something went wrong reading the file!\n");
	  	return ERROR;
	  }
	  if (fscanf(data_ptr, "%*[^:]: %s\n", host) != 1) {
	  	printf("Something went wrong reading the file.\n");
	  	return ERROR;
	  }
	  if (fscanf(data_ptr, "%*[^:]: %s\n", user) != 1) {
	  	printf("Something went wrong reading the file.\n");
	  	return ERROR;
	  }
	  if (fscanf(data_ptr, "%*[^:]: %s\n", pass) != 1) {
	  	printf("Something went wrong reading the file.\n");
	  	return ERROR;
	  }
	  if (fscanf(data_ptr, "%*[^:]: %s\n", db) != 1) {
	  	printf("Something went wrong reading the file.\n");
	  	return ERROR;
	  }
	  if (fscanf(data_ptr, "%*[^:]: %d\n", &port) != 1) {
	  	printf("Something went wrong reading the file.\n");
	  	return ERROR;
	  }
    MYSQL* con = mysql_init(NULL);
	  MYSQL_RES *res;
    MYSQL_ROW row;
	  printf("Connecting...\n");
	  int suc = ERROR;
	  if (!mysql_real_connect(con, host, user, pass, db, port, socket, flags)) {
	  	printf("Creating database if does not exist...\n");
  		char query[100] = {0};
	  	snprintf(query, 100, "CREATE DATABASE IF NOT EXISTS %s", db);
	  	if (mysql_query(con, query)) {
      	printf("Error: %s\n", mysql_error(con));
      	mysql_close(con);
      	suc = ERROR;
      	return ERROR;
    	} else {
    		suc = SUCCESS;
    	}
	  	printf("Creating table if does not exist...\n");
    	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS information (name VARCHAR(30), password VARCHAR(20))")) {
      	printf("Error: %s\n", mysql_error(con));
      	mysql_close(con);
      	suc = ERROR;
      	return ERROR;
    	} else {
    		suc = SUCCESS;
    	}
    	if (suc == -1) {
		  	printf("Error %s (%d)", mysql_error(con), mysql_errno(con));
		  	return ERROR;
		  }
  	}
	  printf("Databse informtion: \n");
	  printf("Host: %s\n", host);
  	printf("user: %s\n", user);
	  printf("password: %s\n", pass);
  	printf("IP/Port: %s/%d\n", socket,port);
 
	  printf("\n");

  /*
  printf("Inserting information (Creating user) \n");
  char *name = "alex";
	char query_info[200] = {0};
	if (check_information(con, name) == USER_AVAIL) {
  	snprintf(query_info, 200, "INSERT INTO information (name, password) SELECT * FROM (SELECT 'alex', '1234') AS tmp WHERE NOT EXISTS (SELECT name FROM information WHERE name = 'alex')");
  	insert_information(con, query_info);
  } 
  else {
  	printf("User already exists\n");
  }


	printf("Test showing information\n");
	show_information(con);

	*/
    mysql_close(con);
    printf("Mysql Server is ready to use, now please use client.c \n");
	  printf("#############\n");
	  return CLOSING_CONN;
	}
} /* main() */

/*
 * This method will insert the information 
 * into the mysql database.
 */

void insert_information(MYSQL *con, char *query) {
	if (mysql_query(con, query)) {
    printf("Error: %s\n", mysql_error(con));
    mysql_close(con);
    return;
  }
  if (mysql_query(con, "SELECT * FROM information")) {
    printf("Error: %s\n", mysql_error(con));
    mysql_close(con);
    return;
  }
} /* insert_information() */

/* 
 * This method will show the imformation of the table.
 */

void show_information(MYSQL *con) {
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) {
    printf("Error: %s\n", mysql_error(con));
    mysql_close(con);
    return;
   }
   MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
   	printf("%s %s\n", row[0], row[1]);
  } 
  mysql_free_result(result);
} /* show_information() */

/*
 * This method will delete the information of a user.
 */

void delete_information(MYSQL *con, char *query) {
	return; 
} /* delete_information() */

/*
 * This method will check if the user exists. 
 */

int check_information(MYSQL *con, char *name) {
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) {
    printf("Error: %s\n", mysql_error(con));
    mysql_close(con);
    return ERROR;
  }
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
   	 if (strcasecmp(row[0], name) == 0) {
   	 	 mysql_free_result(result);
   	 	 return USER_EXISTS;
   	}
  } 
  mysql_free_result(result);
  return USER_AVAIL;
} /* check_information() */
