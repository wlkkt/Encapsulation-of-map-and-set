#pragma once

#include <iostream>
#include<vector>

using namespace std;

enum Colour
{
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode
{
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	pair<K, V> _kv;
	Colour _col;

	RBTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _col(RED)
	{}
};



template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			_root->_col = BLACK;//�涨����ĸ������ɫΪ��ɫ
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(kv);
		cur->_col = RED; // �涨�����ڵ����ɫ�Ǻ�ɫ
		if (parent->_kv.first < kv.first)
		{
			parent->_right = cur;
		}
		else
		{
			parent->_left = cur;
		}
		cur->_parent = parent;

		// ��parentָ��Ľ�㲻Ϊ�գ���parentָ��Ľ�����ɫ��Ϊ��ɫ�ͼ������ϵ�������Ϊcur��parent��Ϊ������������������ϵ������µģ�����������������������һ�֣�
		// ֻ�е�cur��parent����Ϊ��ʱ�Ž���ѭ����
		while (parent && parent->_col == RED)
		{

			Node* grandfather = parent->_parent;//ͨ�������ҵ�үү�����׵ĸ�����Ϣ��үү

			//������Ĺؼ��������ж���������
			if (parent == grandfather->_left)//���������үү�����ӣ���ô�����ֻ����үү���Һ���
				//����if�е�����һֱ����үү���Һ��ӣ������������Ϊ�졢Ϊ�ڡ������ڵ��������
			{
				Node* uncle = grandfather->_right;


				// �������������������ɫ�Ǻ�ɫ����ֱ�ӽ�����͸��׵���ɫ����Ϊ��ɫ����үү����ɫ��Ϊ��ɫ
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					// ��ֹ�޸���ɫ�����Ϸ������µ����������ĺ�ɫ��㣬��Ҫ�ƶ�ָ��������ϴ���
					cur = grandfather;//curָ��үү���
					parent = cur->_parent;//parentָ�����cur�ĸ���㣨cur�����ߣ�parentҲҪ�����ߣ�
				}
				else // ���岻���ڣ����ߴ�����Ϊ��
				{
					if (cur == parent->_left)//���cur�Ǹ��׵����ӣ�����LL�ҵ�����ת�󽫸��׵���ɫ��Ϊ�ڣ���үү����ɫ��Ϊ��
					{
						//     g  
						//   p   u
						// c 
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//      g  
						//   p     u
						//      c 
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}



			else//������׽����үү�����Һ��ӣ���ô���������үү��������
			{
				Node* uncle = grandfather->_left;
				// ���������Ϊ�죬-����ɫ����
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					// �������ϴ���
					cur = grandfather;
					parent = cur->_parent;
				}
				else // ���岻���ڣ����ߴ�����Ϊ��
				{
					// ����������岻���ڻ��ߴ�����Ϊ��
					// ��ת+��ɫ
					//      g
					//   u     p
					//            c
					if (cur == parent->_right)
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//		g
						//   u     p
						//      c
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
		}

		_root->_col = BLACK;//����������ú�����ĸ������ʲô��ɫ����Ϊ���ϵ������ܻᵼ�¸������ɫ��Ϊ��ɫ��Ҳ�п��ܸ����Ҳ��ɫ���䣩
		//�������ﶼֱ�����½���������ɫ����Ϊ��ɫ�������Ͳ���Ҫ����ѭ����ȥ�ж��Ƿ񵽴�����
		return true;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}

			subL->_parent = ppNode;
		}
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		subR->_left = parent;
		Node* ppNode = parent->_parent;

		parent->_parent = subR;

		if (parent == _root)
		{
			_root = subR;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_right == parent)
			{
				ppNode->_right = subR;
			}
			else
			{
				ppNode->_left = subR;
			}
			subR->_parent = ppNode;
		}
	}

	//���û���¶���������������ӿ�
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//�жϺ�����Ƿ�ƽ��
	bool IsBalance()
	{
		//���ȸ������ɫ��Ϊ��
		if (_root->_col == RED)
		{
			return false;
		}

		int refNum = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
			{
				++refNum;
			}

			cur = cur->_left;
		}

		return Check(_root, 0, refNum);
	}

private:
	bool Check(Node* root, int blackNum, const int refNum)
	{
		if (root == nullptr)
		{
			//cout << blackNum << endl;
			if (refNum != blackNum)
			{
				cout << "���ں�ɫ�ڵ����������ȵ�·��" << endl;
				return false;
			}

			return true;
		}

		if (root->_col == RED && root->_parent->_col == RED)
		{
			cout << root->_kv.first << "���������ĺ�ɫ�ڵ�" << endl;
			return false;
		}

		if (root->_col == BLACK)
		{
			blackNum++;
		}

		return Check(root->_left, blackNum, refNum)
			&& Check(root->_right, blackNum, refNum);
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

private:
	Node* _root = nullptr;
};


//���Ժ�����ܷ�����ʹ��
void TestRBTree1()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14,8, 3, 1, 10, 6, 4, 7, 14, 13 };
	RBTree<int, int> t1;
	for (auto e : a)
	{
		if (e == 10)
		{
			int i = 0;
		}

		// 1���ȿ��ǲ���˭���³��ֵ�����
		// 2���������ϵ㣬��������ǰ����
		// 3���������٣��Ա�ͼһһ����ϸ��ԭ��
		t1.Insert({ e,e });

		std::cout << "Insert:" << e << "->" << t1.IsBalance() << std::endl;
	}

	t1.InOrder();

	std::cout << t1.IsBalance() << std::endl;
}

//���Ժ�����԰������ݴ�������ʱ��
void TestRBTree2()
{
	const int N = 1000000;
	vector<int> v;
	v.reserve(N);
	srand(time(0));

	for (size_t i = 0; i < N; i++)
	{
		v.push_back(rand() + i);
		//cout << v.back() << endl;
	}

	size_t begin2 = clock();
	RBTree<int, int> t;
	for (auto e : v)
	{
		t.Insert(make_pair(e, e));
		//cout << "Insert:" << e << "->" << t.IsBalance() << endl;
	}
	size_t end2 = clock();

	std::cout << t.IsBalance() << std::endl;
}