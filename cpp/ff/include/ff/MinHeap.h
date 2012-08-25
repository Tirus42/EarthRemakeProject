#ifndef FF_MIN_HEAP_H_
#define FF_MIN_HEAP_H_

#include <vector>

namespace ff {

class BinaryMinHeap {
 public:
  BinaryMinHeap() : heap_size_(0), nodes_() {}

  // Inserts a new element to the heap.
  void Insert(uint32_t pos, float cost_from_start,
                      float estimated_cost_to_goal) {
    nodes_.push_back(node_t(pos, std::pair<float, float>(cost_from_start, estimated_cost_to_goal)));
    ShiftUp(heap_size_);
    heap_size_++;
  }

  // Removes and returns the minimum element of the heap.
  uint32_t ExtractMin() {
    uint32_t pos = nodes_[0].first;
    if (heap_size_ > 1) {
      heap_size_--;
      Swap(0, heap_size_);
      ShiftDown(0);
    } else {
      heap_size_ = 0;
    }
    nodes_.pop_back();
    return pos;
  }

  // Returns the minimum cost in the heap
  float MinCost() {
    return nodes_[0].second.first;
  }

  // Tries to update the cost of the given node.
  // If the node is not in the heap, it returns 0
  // else if the new cost are not lower than the old it returns -1
  // If the cost was updated, it returns 1
  int UpdateCost(uint32_t pos, float cost_from_start, float estimated_cost_to_goal) {
    uint32_t index = 0;
    for (std::vector<node_t>::iterator i = nodes_.begin(); i != nodes_.end(); i++) {
      if (i->first == pos) {
        if (i->second.second <= estimated_cost_to_goal) {
          return -1;
        }
        i->second.first = cost_from_start;
        i->second.second = estimated_cost_to_goal;
        ShiftUp(index);
        return 1;
      }
      index++;
    }
    return 0;
  }

  // Returns true if the heap is currently empty.
  bool IsEmpty() { return heap_size_ == 0; }

  // Calculates the array index of the left child in a binary tree.
  static uint32_t LeftChildIndex(uint32_t node_index) {
    return 2 * node_index + 1;
  }

  // Calculates the array index of the right child in a binary tree.
  static uint32_t RightChildIndex(uint32_t node_index) {
    return 2 * node_index + 2;
  }

  // Calculates the array index of the parent in a binary tree.
  static uint32_t ParentIndex(uint32_t node_index) {
    return (node_index - 1) / 2;
  }

 private:
  // Swaps two nodes
  inline void Swap(uint32_t node_index1, uint32_t node_index2) {
    std::swap(nodes_[node_index1], nodes_[node_index2]);
  }

  // Heapify method for insertions.
  void ShiftUp(uint32_t node_index) {
    if (node_index > 0) {
      uint32_t parent_index = ParentIndex(node_index);
      if (nodes_[parent_index].second.second > nodes_[node_index].second.second) {
        Swap(node_index, parent_index);
        ShiftUp(parent_index);
      }
    }
  }

  // Heapify method for deletions.
  void ShiftDown(int node_index) {
    uint32_t left_child_index = LeftChildIndex(node_index);
    uint32_t right_child_index = RightChildIndex(node_index);
    uint32_t min_index;

    if (right_child_index >= heap_size_) {
      if (left_child_index >= heap_size_) {
        return;
      } else {
        min_index = left_child_index;
      }
    } else {
      if (nodes_[left_child_index].second.second <= nodes_[right_child_index].second.second) {
        min_index = left_child_index;
      } else {
        min_index = right_child_index;
      }
    }

    if (nodes_[node_index].second.second > nodes_[min_index].second.second) {
      Swap(node_index, min_index);
      ShiftDown(min_index);
    }
  }

  uint32_t heap_size_;

  // first: pos, second.first: cost from start, second.second: estimated cost to goal
  typedef std::pair< uint32_t, std::pair<float, float> > node_t;

  // Array of nodes
  std::vector<node_t> nodes_;
};

} // namespace ff

#endif // MIN_HEAP_H_
