package skipList;

import java.util.Random;

public class skipList {

    private final int MAX_LEVEL = 16;
    private int levelCount = 1;

    private Random random = new Random();

    private Node head = new Node(MAX_LEVEL);

    public Node find(int val) {

        //begin with head node
        Node p = head;
        for (int l = levelCount - 1; l >= 0; l--) {
            while (p.forward[l] != null && p.forward[l].data < val) {
                p = p.forward[l];
            }
        }
        if (p.forward[0] != null && p.forward[0].data == val) {
            return p.forward[0];
        } else {
            return null;
        }

    }

    public void insert(int val) {
        Node p = head;
        //����ǵ�һ������Ľ���ǿ϶��ǵ�һ��
        int level = p.forward[0] == null ? 1 : randomLevel();

        //ֻ����ÿ��������һ��
        if (level > levelCount) {
            level = ++levelCount;
        }

        //��ߴ��ڵ�level��Ľ��Ӧ�ô���һ��ָ���СΪlevel��Node��������,������¼����ĺ�̽ڵ�
        Node newNode = new Node(level);
        newNode.data = val;

        for (int l = levelCount - 1; l >= 0; l--) {
            while (p.forward[l] != null && p.forward[l].data < val) {
                p = p.forward[l];
            }
            //ֻ����level>l�Ĳ���ܹ������½��
            if (level > l) {
                if (p.forward[l] == null) {
                    p.forward[l] = newNode;
                } else {
                    Node next = p.forward[l];
                    p.forward[l] = newNode;
                    newNode.forward[l] = next;
                }
            }
        }
    }


    public void delete(int val) {
        //��һ��levelCount��С�����飬��¼ÿһ�����Ҫɾ���Ľ���ǰһ�����
        Node[] toDeleteNode = new Node[levelCount];
        Node p = head;
        for (int l = levelCount - 1; l >= 0; l--) {
            while (p.forward[l] != null && p.forward[l].data < val) {
                p = p.forward[l];
            }
            toDeleteNode[l] = p;
        }

        //������ԭʼ�����ϵ�p�ĺ�̽ڵ����val�����ҵ�֮��ŷ���
        if(p.forward[0]!=null&&p.forward[0].data==val){
            for (int l = levelCount - 1; l >= 0; l--) {
                if (toDeleteNode[l].forward[l] != null && toDeleteNode[l].forward[l].data == val) {
                    toDeleteNode[l].forward[l] = toDeleteNode[l].forward[l].forward[l];
                }
            }
        }
    }

    /**
     * ����һ���½���ʱ��,��Ҫ��������߿��ܴ��ڵĲ���
     *
     * @return
     */
    private int randomLevel() {
        int level = 1;
        for (int i = 1; i < MAX_LEVEL; i++) {
            if (random.nextInt() % 2 == 1) {
                level++;
            }
        }
        return level;
    }

    class Node {
        private int data;
        private Node[] forward;

        public Node(int level) {
            this.forward = new Node[level];
        }
    }

}


