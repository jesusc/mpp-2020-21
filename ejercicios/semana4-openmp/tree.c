#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>
#include <glib.h>

#include <omp.h>

#define MAX_HEIGHT 4
#define MAX_CHILDREN 4
#define FOLDER_RATIO 4 
// 1/4 are folders => More folders may take more time

#define DEBUG 0

typedef struct {
  char *name;
  long size;
  gboolean is_dir;
} Item;

char * new_name(int height, int child) {
  char str[2048];
  if (DEBUG)
    printf("%d %d\n", height, child);
  sprintf(str, "Node %d %d", height, child);
  return g_strdup(str);
}

Item * item_new(char *name, long size, gboolean is_dir)
{
  Item *item = (Item *) malloc(sizeof(Item));
  item->name = name;
  item->size = size;
  item->is_dir = is_dir;
  return item;
}

void item_free(Item *item)
{
  free(item->name);
  free(item);
}

void generate_nodes(GNode *root, int height) {
  // https://developer.gnome.org/glib/stable/glib-N-ary-Trees.html

  // Generate up to 30 children
  int children = rand() % MAX_CHILDREN + 2;
  for(int j = 0; j < children; j++) {
    int size = 0;
    int is_dir = (rand() % FOLDER_RATIO) == 0; // Some are folders
    if (!is_dir) {
      size = rand() % 500 + 20; 
    }
      
    GNode *node = g_node_new(item_new(new_name(height, j), size, is_dir));
    g_node_append(root, node);

    if (is_dir) {
      // Decide whether to stop.
      // Hard stop if the height is too much. If height is ok, we stop 1/4 of the times
      if (height < MAX_HEIGHT && (rand() % 4) != 0) {
	generate_nodes(node, height + 1);
      }
    }
  }
}

long sum(GNode *root)
{
  long total = 0;
  int nchildren = g_node_n_children(root);
  for(int i = 0; i < nchildren; i++) {
    GNode *child = g_node_nth_child(root, i);
    Item *item = child->data;
    if (item->is_dir) {
      total += sum(child);
    } else {
      total += item->size;
    }
  }
  return total;
}


void free_nodes(GNode *root) {
  item_free(root->data);
  int nchildren = g_node_n_children(root);
  for(int i = 0; i < nchildren; i++) {
    GNode *child = g_node_nth_child(root, i);
    free_nodes(child);
  }
}

void destroy_tree(GNode *root)
{
  free_nodes(root);
  g_node_destroy(root);
}

int main(int argc, char **argv)
{

  GNode *root = g_node_new(item_new(new_name(0, 0), 0, TRUE));

  double start = omp_get_wtime();
  generate_nodes(root, 1);
  double end = omp_get_wtime();
  printf("Generation time %f seconds\n", end - start);

  start = omp_get_wtime();
  long total = sum(root);
  end = omp_get_wtime();
  printf("Total = %ld\n", total);
  printf("Traversal time %f seconds\n", end - start);

  destroy_tree(root);
  
  return 0;  
}
