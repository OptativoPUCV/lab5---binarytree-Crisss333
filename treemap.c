#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    // Reservar memoria para el TreeMap
    TreeMap* map = (TreeMap*)malloc(sizeof(TreeMap)); 
  
    if (map == NULL) {
        return NULL; // Manejo de error si no se puede reservar memoria
    }

    // Inicializar las variables del TreeMap
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;

    //new->lower_than = lower_than;
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

  if (tree == NULL || key == NULL || value == NULL) {
        return; // Manejo de errores si el mapa, la clave o el valor son nulos
  }

    // Paso 1: Realizar una búsqueda para encontrar la ubicación adecuada para el nuevo nodo
  TreeNode* currentNode = tree->root;
  TreeNode* parentNode = NULL;

  while (currentNode != NULL) {
      int comparacion = tree->lower_than(key, currentNode->pair->key);

      if (comparacion == 0) return;
      

      else if (comparacion < 0) {
        parentNode = currentNode;
        currentNode = currentNode->left;       
      } 

      else {
        parentNode = currentNode;
        currentNode = currentNode->right;       
      } 
  }

  TreeNode* newNode = createTreeNode(key, value);

  if (parentNode == NULL) {
      // El árbol estaba vacío, el nuevo nodo será la raíz
      tree->root = newNode;
  } else if (tree->lower_than(key, parentNode->pair->key) < 0) {
      // La clave es menor que la del nodo padre, enlazar en el subárbol izquierdo
      parentNode->left = newNode;
      newNode->parent = parentNode;
  } else {
      // La clave es mayor que la del nodo padre, enlazar en el subárbol derecho
      parentNode->right = newNode;
      newNode->parent = parentNode;
  }

  // Actualizar current para que apunte al nuevo nodo
  tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;

    while (current != NULL) {
        // Compara las claves utilizando la función lower_than
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            // Se encontró la clave, actualiza el puntero 'current' y devuelve el Pair asociado
            tree->current = current;
            return current->pair;
        }
    }

    // La clave no se encontró en el mapa, 'current' sigue siendo NULL
    tree->current = NULL;
    return NULL;
}

/*
{
    if (tree == NULL || key == NULL) {
        return NULL; // Manejo de errores si el mapa o la clave son nulos
    }

    TreeNode* currentNode = tree->root;

    while (currentNode != NULL) {
      int comparacion = tree->lower_than(key, currentNode->pair->key);

      if (comparacion == 0) {
        tree->current = currentNode;
        return currentNode->pair;
      }

      else if (comparacion < 0) currentNode = currentNode->left;

      else currentNode = currentNode->right;
    }
  
    tree->current = NULL;
    return NULL;
}
*/


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
