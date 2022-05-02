#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "binarytree.h"
#include "flattree.h"

TEST_CASE("SUBMISSION", "[weight=1]]")
{
    REQUIRE(true);
}

TEST_CASE("sumdistances::basic","[weight=2][part=tree]"){
	BinaryTree<int> tree;
    tree.insert(0);
    tree.insert(-1, true);
    tree.insert(1, true);
    REQUIRE(tree.sumDistances() == 2);
}

TEST_CASE("sumdistances::stick","[weight=2][part=tree]"){
	BinaryTree<int> stick;
    int sum = 0;
    int cur = 0;
    stick.insert(0);
    for (int i = 1; i < 10; ++i) {
        sum += (++cur);
        stick.insert(i, true); //insert ordered
        REQUIRE(stick.sumDistances() == sum);
    }
    REQUIRE(stick.sumDistances() == 45);
}

TEST_CASE("flattree::getElement","[weight=0][part=flattree]"){
  vector<char> elements = {'A','B','E','C','D'};
  FlatTree<char> tree(elements);
  REQUIRE(tree.getElement("") == 'A');
  REQUIRE(tree.getElement("L") == 'B');
  REQUIRE(tree.getElement("R") == 'E');
  REQUIRE(tree.getElement("LL") == 'C');
  REQUIRE(tree.getElement("LR") == 'D');
}

void checkFlatTree(FlatTree<int>& flat, const vector<int>& expected) {
  REQUIRE(flat.data.size() == expected.size()+1);
  for (int i = 0; i < (int)expected.size(); ++i) {
    REQUIRE(flat.data[i+1] == expected[i]);
  }
}

TEST_CASE("flattree::fromBinaryTree","[weight=0][part=flattree]") {
  BinaryTree<int> tree;  
  FlatTree<int> flat;
    tree.insert(0);
    tree.insert(-2, true);
    tree.insert(2, true);
    {
      flat.fromBinaryTree(tree);
      checkFlatTree(flat, {0,-2,2});
    }
    tree.insert(-1, true);
    tree.insert(-3, true);
    tree.insert(1, true);
    {
      flat.fromBinaryTree(tree);
      checkFlatTree(flat, {0,-2,2,-3,-1,1});
    }
}

TEST_CASE("flattree::toBinaryTree","[weight=0][part=flattree]") {
  BinaryTree<int> tree;  
  FlatTree<int> flat, flat2;
    tree.insert(0);
    tree.insert(-2, true);
    tree.insert(2, true);
    {
      flat.fromBinaryTree(tree);
      flat2.fromBinaryTree(flat.toBinaryTree());
      REQUIRE(flat2.data.size() == 4);
      REQUIRE(flat.data == flat2.data);
    }
    tree.insert(-1, true);
    tree.insert(-3, true);
    tree.insert(1, true);
    {
      flat.fromBinaryTree(tree);
      flat2.fromBinaryTree(flat.toBinaryTree());
      REQUIRE(flat2.data.size() == 7);
      REQUIRE(flat.data == flat2.data);
    }
}

