#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 256
#define NUM_BUCKETS 100

typedef struct customer {
  char *email;
  char *name;
  char *shoe_size;
  char *fav_food;
  struct customer *next;
} customer;

unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while (*str != '\0') {
    c = *str;
    hash = ((hash << 5) + hash) + (unsigned char)c;
    str++;
  }
  return hash;
}

customer *add_customer_to_list(char *email, char *name, char *shoe_size,
                               char *fav_food, customer *bucket) {
  customer *new_customer;
  new_customer = malloc(sizeof(customer));
  new_customer->email = strdup(email);
  new_customer->name = strdup(name);
  new_customer->shoe_size = strdup(shoe_size);
  new_customer->fav_food = strdup(fav_food);
  new_customer->next = bucket;
  return new_customer;
}

void add_customer(char *email, char *name, char *shoe_size, char *fav_food,
                  customer **buckets, size_t num_buckets) {
  size_t which_bucket = hash(email) % num_buckets;
  customer *node;
  node = buckets[which_bucket];
  bool added = false;
  while (node != NULL) {
    if (strcmp(node->email, email) ==
        0) { // if email is already present then replace the data that was
             // already in the hash table with this new entry
      node->name = strdup(name);
      node->shoe_size = strdup(shoe_size);
      node->fav_food = strdup(fav_food);
      added = true;
    }
    node = node->next;
  }
  if (added != true) {
    buckets[which_bucket] = add_customer_to_list(
        email, name, shoe_size, fav_food, buckets[which_bucket]);
  }
}

void *lookup(char *email, customer **buckets, size_t num_buckets) {
  size_t which_bucket = hash(email) % num_buckets;
  bool email_found = false;
  customer *node;
  node = buckets[which_bucket];

  while (node != NULL) {
    if (strcmp(node->email, email) == 0) {
      printf("Name: %s ", node->name);
      printf("Shoe size: %s ", node->shoe_size);
      printf("Favorite food: %s ", node->fav_food);
      printf("\n");
      email_found = true;
    }
    node = node->next;
  }
  if (email_found == false) { // email is not in the hash table
    printf("Customer was not found\n");
  }
  return 0;
}

bool not_found(char *email, customer **buckets,
               size_t num_buckets) { // helper used to find out if email exists
  size_t which_bucket = hash(email) % num_buckets;
  customer *node;
  node = buckets[which_bucket];
  while (node != NULL) {
    if (strcmp(node->email, email) == 0) {
      return false;
    }
    node = node->next;
  }
  return true;
}

void delete (char *email, customer **buckets, size_t num_buckets) {
  size_t which_bucket = hash(email) % num_buckets;
  customer *node;
  node = buckets[which_bucket];
  customer *current = node; // current node we are working on to delete
  if (not_found(email, buckets, num_buckets) == false) {
    if (strcmp(node->email, email) == 0) {
      current = node;               // we want to free/delete this node
      node = node->next;            // we want to point to the next node
      buckets[which_bucket] = node; // sets the bucket to the next node
      free(current);                // free it
      current = NULL;               // set it to null
    } else {
      customer *temp = node;
      while (temp->next != NULL) {
        if (strcmp(temp->next->email, email) == 0) {
          current = temp->next;
          temp->next = temp->next->next;
          free(current);
        }
        temp = temp->next;
      }
    }
  } else if (not_found(email, buckets, num_buckets) == true) {
    printf("Customer was not found\n");
  }
}

void list(customer **buckets, size_t num_buckets) {
  for (size_t i = 0; i < num_buckets; i++) {
    customer *node = buckets[i];
    while (node != NULL) {
      printf("Email Address: %s\n", node->email);
      printf("Name: %s\n", node->name);
      printf("Shoe size: %s\n", node->shoe_size);
      printf("Favorite food: %s\n", node->fav_food);
      printf("\n");
      node = node->next;
    }
  }
}

void load(char *filename, customer **buckets, size_t num_buckets) {
  char email[256];
  char name[256];
  char shoe_size[256];
  char fav_food[256];
  char buf[BUFSIZE];
  FILE *infile = fopen(filename, "r");
  while (fgets(buf, BUFSIZE, infile)) {
    sscanf(buf, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n", email, name, shoe_size,
           fav_food);
    add_customer(email, name, shoe_size, fav_food, buckets, num_buckets);
  }
  fclose(infile);
}
void save(char *filename, customer **buckets, size_t num_buckets) {
  FILE *infile = fopen(filename, "w");
  for (size_t i = 0; i < num_buckets; i++) {
    customer *node = buckets[i];
    while (node != NULL) {
      fprintf(infile, "%s\t%s\t%s\t%s\n", node->email, node->name,
              node->shoe_size, node->fav_food);
      node = node->next; // goes to the next person
    }
  }
  fclose(infile); // no leaks please :(
}

void free_all(customer **buckets,
              size_t num_buckets) { // somewhat similar to list but free values
  for (size_t i = 0; i < num_buckets; i++) {
    customer *current = buckets[i];
    while (current != NULL) {
      free(current->email);
      free(current->name);
      free(current->shoe_size);
      free(current->fav_food);

      customer *temp = current; // uses temp to free the current node later
      free(temp);
      current = current->next; // goes to the next one
    }
  }
}

int main(void) {
  customer *buckets[NUM_BUCKETS] = {NULL};
  load("customers.tsv", buckets, NUM_BUCKETS);
  while (1) {
    char word[10];
    printf("Please type a command: ");
    fgets(word, sizeof(word), stdin);

    if (strncmp(word, "exit", 4) == 0) {
      free_all(buckets, NUM_BUCKETS);
      printf("exiting out and freeing memory\n");
      break;
    }
    if (strncmp(word, "add", 3) == 0) {
      char email[256];
      char name[256];
      char shoe_size[256];
      char fav_food[256];

      printf("Email Address: ");
      fgets(email, sizeof(email), stdin);
      printf("Name: ");
      fgets(name, sizeof(name), stdin);
      printf("Shoe size: ");
      fgets(shoe_size, sizeof(shoe_size), stdin);
      printf("Favorite food: ");
      fgets(fav_food, sizeof(fav_food), stdin);
      email[strlen(email) - 1] = 0; // removes the new line
      name[strlen(name) - 1] = 0;
      shoe_size[strlen(shoe_size) - 1] = 0;
      fav_food[strlen(fav_food) - 1] = 0;
      add_customer(email, name, shoe_size, fav_food, buckets, NUM_BUCKETS);
    }
    if (strncmp(word, "lookup", 6) == 0) {
      char lookup_email[256];
      printf("Email Address: ");
      fgets(lookup_email, sizeof(lookup_email), stdin);
      lookup_email[strlen(lookup_email) - 1] = 0;
      lookup(lookup_email, buckets, NUM_BUCKETS);
    }
    if (strncmp(word, "list", 4) == 0) {
      list(buckets, NUM_BUCKETS);
    }
    if (strncmp(word, "delete", 6) == 0) {
      char delete_email[256];
      printf("Email Address: ");
      fgets(delete_email, sizeof(delete_email), stdin);
      delete_email[strlen(delete_email) - 1] = 0;
      delete (delete_email, buckets, NUM_BUCKETS);
    }
    if (strncmp(word, "save", 4) == 0) {
      save("customers.tsv", buckets, NUM_BUCKETS);
      printf("Saved\n");
    }
  }
  return 0;
}
