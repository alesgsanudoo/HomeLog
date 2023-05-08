/* 
 * A really simple Log-in, log-out system
 * with mysql connection.
 *
 * This class will contain everything
 * related to mysql connection.
 *
 * Created by Alejandro S, 05/06/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

void show_information(MYSQL *con);
void insert_information(MYSQL *con, char *query);
void delete_information(MYSQL *con, char *query);

static char *host = "localhost";
static char *user = "root";
static char *pass = "Agrisa22!";
static char *db   = "test";
static char *socket = NULL;
unsigned int port = 3306;
unsigned int flags = 0;


/* 
 * The main will create the database and tables in 
 * order to make the program work correctly. 
 */

int main(int argc, char *argv[]) {	
	MYSQL* con = mysql_init(NULL);
	MYSQL_RES *res;
  MYSQL_ROW row;

	printf("#######################\n");
	printf("Connecting...\n");
	if (!mysql_real_connect(con, host, user, pass, db, port, socket, flags)) {
		printf("Error %s (%d)", mysql_error(con), mysql_errno(con));
		return 0;
	}
	printf("Databse informtion: \n");
	printf("Host: %s\n", host);
	printf("user: %s\n", user);
	printf("password: %s\n", pass);
	printf("IP/Port: %s/%d\n", socket,port);

	printf("\n");

	printf("Creating database if does not exist...\n");
	char query[100] = {0};
	snprintf(query, 100, "CREATE DATABASE IF NOT EXISTS %s", db);
	if (mysql_query(con, query)) {
    printf("Error: %s\n", mysql_error(con));
    mysql_close(con);
    return 0;
  } 
	
	printf("Creating table if does not exist...\n");

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS information (name VARCHAR(30), password VARCHAR(20))")) {
    printf("Error: %s\n", mysql_error(con));
    mysql_close(con);
    return 0;
   }

  printf("Inserting information (Creating user) \n");
	char query_info[200] = {0};
  snprintf(query_info, 200, "INSERT INTO information (name, password) SELECT * FROM (SELECT 'alex', '1234') AS tmp WHERE NOT EXISTS (SELECT name FROM information WHERE name = 'alex')");
  insert_information(con, query_info);


	printf("Test showing information\n");
	show_information(con);

  /* close connection */
  mysql_close(con);

	printf("#############\n");
	
	return 0;
} /* main() */


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
}

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

void delete_information(MYSQL *con, char *query) {
	return; 
}
