#include <stdio.h>
#include <math.h>

//-> ���� x����
//����� y����
//�� ��Ʈ������ ù��° ������Ʈ, �� ������ (1,1)
//���� ������ �������� ������ ��.
//����� ������ �� �ִ� �����--------------------------------------
#define MAPSIZE_X		7										//�ʻ����� X��
#define MAPSIZE_Y		5										//�ʻ����� Y��
#define STARTPOINT_X	2										//�������� X��
#define STARTPOINT_Y	3										//�������� Y��
#define DESTINATION_X	6										//�������� X��
#define DESTINATION_Y	3										//�������� Y��
const int WALLLIST[][2] = { {4, 2}, {4, 3}, {4, 4} };			//������ �� ���� WALL������ ��ǥ. �� x��, y���� ��ǥ�Ѵ�.
//����� ������ �� �ִ� ����� ��-----------------------------------



//�� �� ������ �Ÿ��� ����.
#define GET_DISTANCE(X1, Y1, X2, Y2) ( sqrt( pow(X1-X2, 2) + pow(Y1-Y2, 2) ) )
//(x, y) ��ǥ���� ��ǻ�Ϳ� ����� �迭�� �°� ��ȯ�Ѵ�.
#define ARRAY(MAP, X, Y)			( MAP[Y-1][X-1] )
//�ش� ���� ��ǥ�� PASSABLE�ΰ�?
#define IS_PASSABLE(MAP, X, Y) ( ARRAY(MAP, X, Y).type == PASSABLE ? true : false )
//�ش� ����Ʈ�� �����ϴ°�? �ִٸ� OUT���� �ش� ��ũ������ �ּҰ���, ���� ��� null���� ����.
#define IS_ONLIST(LISTHEAD, NODEPTR, OUT)	OUT = LISTHEAD; while(OUT){if(OUT->data->x == NODEPTR->x && OUT->data->y == NODEPTR->y) break;OUT = OUT->next;}


//Ÿ��. �׷��� ȯ�濡�� �� ���� �̿��� �׷��� ǥ���� ������.
enum nodetype { STARTPOINT, DESTINATION, WALL, PASSABLE };
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

	nodetype type = (nodetype)0;

	//���� (x,y) 2���� �̿��� �� �� ������ �Ÿ��� ���� g, h�� ����.
	//void SetG(int start_x, int start_y) { g = GET_DISTANCE(x, y, start_x, start_y); }
	//void SetH(int dest_x, int dest_y) { h = GET_DISTANCE(x, y, dest_x, dest_y); }
	void RefreshG(void) { g = GET_DISTANCE(x, y, STARTPOINT_X, STARTPOINT_Y); }
	void RefreshH(void) { h = GET_DISTANCE(x, y, DESTINATION_X, DESTINATION_Y); }
	void RefreshF(void) { f = g + h; }
	void RefreshGHF(void) { RefreshG(); RefreshH(); RefreshF(); }
	node() {}
	virtual ~node() {}
};
struct linkednode
{
	node *data = nullptr;
	linkednode *next = nullptr;
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
	int walllistsize = sizeof(WALLLIST) / (sizeof(int) * 2);
	for (int i = 0; i < walllistsize; i++)
		ARRAY(map, WALLLIST[i][0], WALLLIST[i][1]).type = WALL;

	//����� ���� �� openlist�� �߰�.
	openlisthead->data->x = STARTPOINT_X;
	openlisthead->data->y = STARTPOINT_Y;
	openlisthead->data->RefreshGHF();
	openlisthead->next = nullptr;



	linkednode *temp = nullptr;
	//�� while ���� ���� best��θ� ã���� �� break ó����.
	//���� �������� ��� failed�� ����ϸ� break.
	//openlist�� ������� ��� ������ ��θ� ã�Ҵٰ� ������.
	//�׷��� ã�� ���� ��δ� closelist�� ����Ǹ�,
	//�� ����Ʈ�� f���� ����Ͽ� �������ϸ� ���� ��ΰ� ���´�.
	while (openlisthead)
	{

		temp = openlisthead;
		openlisthead = openlisthead->next;//���� ���� ����Ʈ�� �����Ѵ�.
		temp->next = closelisthead;
		closelisthead = temp;	//�ᱹ closelisthead�� ���� Ž������ ��带 ����Ű��, �� �ڷ� ������� ����� ������ �̾�����.
								//���⼭ ���� �������� ������ ��� �� ����Ʈ�� �״�� ������ ��θ� ���� �ڷᰡ �ȴ�.
								//�׷��� ���� �� ���� null�� ��� ������ ��θ� ã�� �� �����ϰ� �ȴ�.
		if (!closelisthead)
		{
			printf("FAILED!\n");
			break;
		}
		if (closelisthead->data->x == DESTINATION_X - 1 && closelisthead->data->y == DESTINATION_Y - 1)
		{
			printf("SUCCESS!\n");
			break;
		}


	}

	//while ���� ���� ��ΰ��� ����Ǹ�, �̴� closelisthead�� ����Ǿ� �ִ�.
	//�ٸ� ���������κ��� ������������ ����, �� �������̱� ������
	//�̸� �ݴ�� �ٲ��� �ʿ䰡 �ִ�. linkednode�� *prev�� �߰��Ͽ� ���������ε� �� �� �ֵ��� �ϰų�
	//������ ��带 �ٲٰų� �����.

	//����
	DeleteLinkedNode(openlisthead);
	DeleteLinkedNode(closelisthead);
	DeleteMap(map, 7, 5);
	return 0;
}