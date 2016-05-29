#include <stdio.h>
#include <math.h>

#define STARTPOINT_X	2
#define STARTPOINT_Y	3
#define DESTINATION_X	6
#define DESTINATION_Y	3

//�� �� ������ �Ÿ��� ����.
#define GET_DISTANCE(X1, Y1, X2, Y2) ( sqrt( pow(X1-X2, 2) + pow(Y1-Y2, 2) ) )
#define ARRAY(MAP, X, Y)			( MAP[Y-1][X-1] )
//Ÿ��. �׷��� ȯ�濡�� �� ���� �̿��� �׷��� ǥ���� ������.
enum nodetype { STARTPOINT, DESTINATION, WALL, PASSABLE };

//�ش� ���� ��ǥ�� PASSABLE�ΰ�?
#define IS_PASSABLE(MAP, X, Y) ( ARRAY(MAP, X, Y).type == PASSABLE ? true : false )


class node
{
public:
	//����	���������κ����� �Ÿ�,	�������κ����� �Ÿ�,	�������� �ǹ� 
	float	g = 0,				h = 0,				f = 0;
	//��ġ
	int x = 0, y = 0;
	//Ű�е��� ���ڸ� �״�� �������� ����� ��.
	//���� direction[7]�� ���� ����� �ǹ�.
	//���� 5�� 0�� ���� ����.(�� ��帶�� (node ������ ũ��)*2 �� ���������, ���� ��ǻ�Ϳ����� �̹��� ����.)
	node *direction[10] = { 0 };

	nodetype type;

	//���� (x,y) 2���� �̿��� �� �� ������ �Ÿ��� ���� g, h�� ����.
	//void SetG(int start_x, int start_y) { g = GET_DISTANCE(x, y, start_x, start_y); }
	//void SetH(int dest_x, int dest_y) { h = GET_DISTANCE(x, y, dest_x, dest_y); }
	void SetG(void) { g = GET_DISTANCE(x, y, STARTPOINT_X, STARTPOINT_Y); }
	void SetH(void) { h = GET_DISTANCE(x, y, DESTINATION_X, DESTINATION_Y); }
	void RefreshF() { f = g + h; }
	node() {}
	virtual ~node() {}
};

//��ũ�� ����Ʈ.
struct linkednode
{
	node *data;
	//linkednode *prev;
	linkednode *next;
};


node **InitializeMap(int x, int y)
{
	node **temp = new node*[y];
	for (int i = 0; i < y; i++)
	{
		temp[i] = new node[x];
		for (int j = 0; j < x; j++)
		{
			//���� PASSABLE�� �ʱ�ȭ.
			temp[i][j].type = PASSABLE;
		}
	}
	return temp;
}

void DeleteMap(node **map, int x, int y)
{
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
			delete (map[i] + j);
		delete map[i];
	}
	delete map;
}
void DeleteLinkedNode(linkednode *ln)
{
	linkednode *temp = ln;
	while (temp)
	{
		temp = ln->next;
		delete ln;
		ln = temp;
	}
}

//�ش� list�� n�� �ִ��� Ȯ��.
//Ȯ���ϴ� ����� ����Ʈ ���� ����� n���� x,y���� ���Ѵ�.
bool IsOnList(linkednode list, node n)
{
	linkednode *temp = &list;
	while (temp)
	{
		if (temp->data->x == n.x && temp->data->y == n.y)
			return true;
		temp = temp->next;
	}
	return false;
}



int main(void)
{
	//�� �ʱ�ȭ
	node **map = InitializeMap(7, 5);

	//open ����Ʈ�� ��尪. ����Ű�� ���� ������.
	linkednode *openlisthead = new linkednode;//open ����Ʈ�� �Ӹ�. ù ���� �κ��� ���� open ����Ʈ�� ���Ե�.
	linkednode *closelisthead = nullptr;//close ����Ʈ�� �Ӹ�. ����� �������.
	
	//�� ����
	ARRAY(map, STARTPOINT_X, STARTPOINT_Y).type = STARTPOINT;
	ARRAY(map, DESTINATION_X, DESTINATION_Y).type = DESTINATION;
	ARRAY(map, 4, 2).type = WALL;
	ARRAY(map, 4, 3).type = WALL;
	ARRAY(map, 4, 4).type = WALL;

	//����� ���� �� openlist�� �߰�.
	openlisthead->data->x = 2;
	openlisthead->data->y = 3;
	openlisthead->data->SetG();
	openlisthead->data->SetH();
	openlisthead->data->RefreshF();
	openlisthead->next = nullptr;
	//openlisthead->prev = nullptr;



	linkednode *best = nullptr;

	//�� while ���� ���� best��θ� ã���� �� break ó����.
	//���� �������� ��� failed�� ����ϸ� break.
	//openlist�� ������� ��� ������ ��θ� ã�Ҵٰ� ������.
	while (openlisthead)
	{
		//best�� open�� �Ӹ��� ����Ŵ���ν�
		//���� open����Ʈ�� ������Ʈ�� ������.
		//���� openlist�� best�� �������� ������ �� ���������� ����(�����) ����Ʈ�� ���Խ�Ŵ.
		//�׸��� best�� �������� close ����Ʈ�� �ٽ� ����Ŵ���ν�
		//best->next �� openlisthead->closelisthead�� �Ǿ���.
		//�������� �𸣰ڴ�.

		//������� 
		best = openlisthead;
		openlisthead = openlisthead->next;
		best->next = closelisthead;

		closelisthead = best; //best(�� ���� ���)�� ���ݱ����� ����(���� ����)���� ��� Ž���ǹǷ� close����Ʈ�� ���Ե�.
	}

	//while ���� ���� best���� �����.

	//����
	DeleteLinkedNode(openlisthead);
	DeleteLinkedNode(closelisthead);
	DeleteMap(map, 7, 5);
	return 0;
}