/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

#include "kdtree.h"

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    //std::cout << first[curDim] << " " << second[curDim] << std::endl;
    /*
    if (first[curDim] != second[curDim]) {
      return first[curDim] < second[curDim];
    } else {
      return first < second;
    }*/
    if (first[curDim] < second[curDim]) { return true; }
    if (first[curDim] > second[curDim]) { return false; }
    return first < second;

    //return false;
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
     double currDist = 0.0;
     double potentialDist = 0.0;
     for (int i = 0; i < Dim; i++) {
      currDist = currDist + (target[i] - currentBest[i])*(target[i] - currentBest[i]);
      potentialDist = potentialDist + (target[i] - potential[i])*(target[i] - potential[i]);
     }
     if (currDist != potentialDist) { return potentialDist < currDist; } 
     else { return potential < currentBest; }
     //eturn false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    
    if (newPoints.size() == 0) { return; }
    root = nullptr;
    std::vector<Point<Dim>> points = newPoints;
    int dim = 0;
    auto start = points.begin();
    auto end = points.end();
    /*
    int mid;
    if (points.size() % 2 == 1) {
      mid = (points.size()-1)/2;
    } else {
      mid = points.size()/2 - 1;
    }
    auto middle = start + mid;*/
    BuildRecursive(start, end, dim, root);
    
    
}

template <int Dim>
template <typename RandIter>
void KDTree<Dim>::BuildRecursive(RandIter start, RandIter end, int dim, KDTreeNode*& tmp) {
  if (!(start < end)) { return; }

  int size = static_cast<int>(end - start);
  int mid = (size - 1) / 2;
  RandIter middle = start + mid;
  auto cmp = [dim](Point<Dim> lhs, Point<Dim> rhs ){return smallerDimVal(lhs, rhs, dim);};
  //dim = (dim + 1) % Dim;
  select(start, end, middle, cmp);
  //end = middle - 1;
  tmp = new KDTreeNode(*(middle));
  if (mid % 2 == 1) {
    mid = (mid-1)/2;
  } else {
    mid = mid - 1;
  }

  int nextDim = (dim + 1) % Dim;
  
  BuildRecursive(start, middle, nextDim, tmp->left);
  BuildRecursive(middle + 1, end, nextDim, tmp->right);
}




template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    if (root == NULL) {
      return Point<Dim>();
    }
    
    // Start with root as current best
    KDTreeNode* bestNode = root;
    return NeighborRecursive(query, root, 0, bestNode)->point;
    /*
    Point<Dim> q = query;
    double best = 0.0;
    for (int i = 0; i < Dim; i++) {
      best += (query[i] - root->point[i])*(query[i] - root->point[i]);
    }
    const Point<Dim> ans = NeighborRecursive(q, root, 0, best, root)->point;
    return ans;*/
}

//template <int Dim>
//typename KDTree<Dim>::KDTreeNode*
/*
KDTree<Dim>::NeighborRecursive(Point<Dim> query, typename KDTree<Dim>::KDTreeNode* node, int dim, double currBest, KDTree<Dim>::KDTreeNode* bestNode ) const {
  if (!node) { return bestNode; }
  if (node->point == query) { return node; }
  if (bestNode == root) {
    if (smallerDimVal(query, node->point, dim)) {
      if (!node->left) {
        double best = 0.0;
        for (int i = 0; i < Dim; i++) { best += (query[i] - node->point[i])*(query[i] - node->point[i]); }
        currBest = best;
        bestNode = node;
        return bestNode;
      } else {
        bestNode = NeighborRecursive(query, node->left, (dim+1)%Dim, currBest, bestNode);
      }
    } else {
      if (!node->right) {
        double best = 0.0;
        for (int i = 0; i < Dim; i++) { best += (query[i] - node->point[i])*(query[i] - node->point[i]); }
        currBest = best;
        bestNode = node;
        return bestNode;
      } else {
        bestNode = NeighborRecursive(query, node->right, (dim+1)%Dim, currBest, bestNode);
      }
    }
  }

  //if (node == root && bestNode != root) { return bestNode;}
  //std::cout << node->point[0] << std::endl;
  //std::cout << "checked bare minimum" << std::endl;
  double best = 0.0;
  for (int i = 0; i < Dim; i++) {
    best += (query[i] - node->point[i])*(query[i] - node->point[i]);
  }
  if (best < currBest || (best == currBest && smallerDimVal(node->point, bestNode->point, dim))) {
    currBest = best;
    bestNode = node;
  }
  if (best == currBest) {
    bool straight = true;
    for (int i = 0; i < Dim; i++) {
      if (i == dim) { continue; }
      if (node->point[i] != query[i]) { straight = false; }
    }
    if (!straight) {
      
    }
  }
  //std::cout << "checked if this node better" << std::endl;
  KDTreeNode* left = NeighborRecursive(query, node->left, (dim+1)%Dim, currBest, bestNode);
  //std::cout << "checked left" << std::endl;
  KDTreeNode* right = NeighborRecursive(query, node->right, (dim+1)%Dim, currBest, bestNode);
  double l = 0.0;
  double r = 0.0;
  for ( int i = 0; i < Dim; i++) {
    l += (query[i] - left->point[i])*(query[i] - left->point[i]);
    r += (query[i] - right->point[i])*(query[i] - right->point[i]);
  }
  //std::cout << "checking both left and right" << std::endl;
  if (l < r) { return left; }
  if (r < l) { return right; }
  if (smallerDimVal(left->point, right->point, dim)) { return left; }
  else { return right; }
  

}*/

template <int Dim>
typename KDTree<Dim>::KDTreeNode*
KDTree<Dim>::NeighborRecursive(const Point<Dim>& query, KDTree<Dim>::KDTreeNode* node, 
                                int dim, KDTree<Dim>::KDTreeNode*& bestNode) const {
    if (node == NULL) {
        return bestNode;
    }
    
    // Save next dimension
    int nextDim = (dim + 1) % Dim;
    
    // Determine which subtree to search first
    KDTreeNode* firstSubtree = NULL;
    KDTreeNode* secondSubtree = NULL;
    
    if (smallerDimVal(query, node->point, dim)) {
        firstSubtree = node->left;
        secondSubtree = node->right;
    } else {
        firstSubtree = node->right;
        secondSubtree = node->left;
    }
    
    // Recursively search the primary subtree
    if (firstSubtree != NULL) {
        bestNode = NeighborRecursive(query, firstSubtree, nextDim, bestNode);
    }
    
    // Check if current node is better than current best
    if (shouldReplace(query, bestNode->point, node->point)) {
        bestNode = node;
    }
    
    // Calculate squared distance from query to current best
    double bestDist = 0.0;
    for (int i = 0; i < Dim; i++) {
        double diff = query[i] - bestNode->point[i];
        bestDist += diff * diff;
    }
    
    // Calculate distance from query to splitting plane
    double planeDist = (query[dim] - node->point[dim]) * (query[dim] - node->point[dim]);
    
    // If the hypersphere crosses the splitting plane, search the other subtree
    if (planeDist <= bestDist && secondSubtree != NULL) {
        bestNode = NeighborRecursive(query, secondSubtree, nextDim, bestNode);
    }
    
    return bestNode;
}



template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
    * @todo Implement this function!
    */ 
  while (start < end) {  
    // Choose pivot (middle element for simplicity)
    RandIter pivot = start + (end - start) / 2;
    auto pivotVal = *pivot;

    // Partition around pivot
    RandIter right = end - 1;
    std::iter_swap(pivot, right); // move pivot to end
    RandIter store = start; 
    for (RandIter it = start; it < right; ++it) {
      if (cmp(*it, pivotVal)) { 
        std::iter_swap(it, store);
        ++store;
      } 
    }

    std::iter_swap(store, right); // place pivot in final position

    // store is now pivot position 
    if (k == store) { return; } // found kth element
    else if (k < store) { end = store; } // search left side 
    else { start = store + 1; } // search right side 
  }
}

