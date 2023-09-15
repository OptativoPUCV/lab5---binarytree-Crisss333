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
  TreeNode* current = tree->root;
  TreeNode* parent = NULL;

  while (current != NULL) {
      parent = current;
  
      // Compara las claves utilizando la función lower_than
      if (tree->lower_than(key, current->pair->key)) {
          current = current->left;
      } else if (tree->lower_than(current->pair->key, key)) {
          current = current->right;
      } else {
          // La clave ya existe en el mapa, no hagas nada y simplemente actualiza 'current'
          tree->current = current;
          return;
      }
  }

  TreeNode* newNode = createTreeNode(key, value);

  if (newNode == NULL) {
      // Manejo de error si no se pudo crear el nuevo nodo
      return;
  }

  // Enlaza el nuevo nodo con el padre
  newNode->parent = parent;

  // Decide si el nuevo nodo es un hijo izquierdo o derecho
  if (tree->lower_than(key, parent->pair->key)) {
      parent->left = newNode;
  } else {
      parent->right = newNode;
  }

  // Actualiza el puntero 'current' para que apunte al nuevo nodo insertado
  tree->current = newNode;
}


TreeNode * minimum(TreeNode * x){
    // Si x no tiene hijo izquierdo, x es el nodo con la clave mínima
    if (x == NULL) return NULL;

    while (x->left != NULL) x = x->left;

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || tree->root == NULL || node == NULL) {
        return;
    }

    // Caso 1: Nodo sin hijos (hoja)
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            // Es el nodo raíz, simplemente lo eliminamos
            free(node->pair);
            free(node);
            tree->root = NULL;
        } else if (node->parent->left == node) {
            // El nodo es un hijo izquierdo
            node->parent->left = NULL;
            free(node->pair);
            free(node);
        } else {
            // El nodo es un hijo derecho
            node->parent->right = NULL;
            free(node->pair);
            free(node);
        }
    }

    // Caso 2: Nodo con un hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (node->parent == NULL) {
            // Es el nodo raíz, reemplazamos con el hijo y liberamos el nodo
            tree->root = child;
            child->parent = NULL;
            free(node->pair);
            free(node);
        } else if (node->parent->left == node) {
            // El nodo es un hijo izquierdo
            node->parent->left = child;
            child->parent = node->parent;
            free(node->pair);
            free(node);
        } else {
            // El nodo es un hijo derecho
            node->parent->right = child;
            child->parent = node->parent;
            free(node->pair);
            free(node);
        }
    }

    // Caso 3: Nodo con dos hijos
    else {
        TreeNode* successor = minimum(node->right);
        // Copiamos los datos del nodo mínimo (successor) al nodo actual (node)
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;
        // Eliminamos el nodo mínimo (successor) recursivamente
        removeNode(tree, successor);
    }
}
  

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        // Manejo de error si el TreeMap o el árbol están vacíos
        tree->current = NULL;
        return NULL;
    }
  
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

    // Si la clave no se encontró en el mapa, el 'current' sigue siendo NULL
    tree->current = NULL;
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }

    TreeNode* current = tree->root;
    Pair* ub_pair = NULL;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            // La clave actual es mayor que 'key', actualiza 'ub_pair' y ve a la izquierda
            ub_pair = current->pair;
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            // La clave actual es menor que 'key', ve a la derecha
            current = current->right;
        } else {
            // Se encontró una clave igual a 'key', devuelve el par asociado
            return current->pair;
        }
    }

    if (ub_pair != NULL) {
        return ub_pair;
    }

    return NULL;
}



/*
{
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;

    while (current != NULL) {
        // Compara las claves utilizando la función lower_than
        if (tree->lower_than(key, current->pair->key)) {
            // La clave actual es mayor que 'key', actualiza 'ub_node' y ve a la izquierda
            ub_node = current;
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            // La clave actual es menor que 'key', ve a la derecha
            current = current->right;
        } else {
            // Se encontró una clave igual a 'key', devuelve el par asociado
            return current->pair;
        }

    if (ub_node != NULL) return ub_node->pair;

    return NULL;
}
*/

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode * current = minimum(tree->root);

    if(current != NULL) {
        tree->current = current;
        return current->pair;
    }
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    
    TreeNode * current = tree->current->right;

    if (current != NULL) {
        current = minimum(current);
        tree->current = current;
        return current->pair;
    }

    TreeNode * parent = tree->current->parent;

    while (parent != NULL && tree->current == parent->right) {
        tree->current = parent;
        parent = parent->parent;
    }

    if (parent != NULL) {
        tree->current = parent;
        return parent->pair;
    }

    return NULL;
}
