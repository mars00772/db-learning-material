package BitMap;

import java.util.*;

public class BitMap {

    //ÿ��word��ռ��bit Ĭ��Ϊ64
    private static final int BITS_PER_WORD = 64;

    //long 64bit = 1 << 6
    private static final int ADDRESS_BITS_PER_WORD = 6;

    //��long���͵�word���洢bit�����Ծ����ܼ���word����
    private long[] words;

    //Ĭ��ÿ��word�洢64bit
    //Ĭ�Ϲ���һ��bit
    public BitMap() {
        initWords(BITS_PER_WORD);
    }

    public BitMap(int bitsPerWord){
        initWords(bitsPerWord);
    }

    private void initWords(int bitsPerWord) {
        int size = wordIndex(bitsPerWord -1);
        this.words = new long[size + 1];
    }

    private int wordIndex(int bits) {
        return bits >> 6;
    }

    //get����
    //�ж�λͼ��bitIndex��Ӧ��bit�Ƿ�Ϊ1 bitIndex�����������
    public boolean get(int bitIndex) {
        if (bitIndex < 0){
            throw new IndexOutOfBoundsException("bitIndex < 0: " + bitIndex);
        }
        //Ѱ�Ҵ洢��bit�Ķ�Ӧ��word
        int wordIndex = wordIndex(bitIndex);
        // �ж϶�Ӧ��bit��wordIndex�Ƿ�Խ��&������жϸ�λ�Ƿ�Ϊ1
        return (wordIndex < words.length) && ((this.words[wordIndex] & (1L << bitIndex)) != 0);
    }

    //set���� ָ��bitIndex��Ϊ1
    public void set(int bitIndex) {
        if (bitIndex < 0){
            throw new IndexOutOfBoundsException("bitIndex < 0: " + bitIndex);
        }
        //Ѱ�Ҵ洢��bit�Ķ�Ӧ��word
        int wordIndex = wordIndex(bitIndex);

        //�������� ȷ��words�ܹ����wordIndex + 1 ��bit
        tryExpand(wordIndex);

        this.words[wordIndex] |= (1L << bitIndex);
    }

    public void flip(int bitIndex) {
        if (bitIndex < 0){
            throw new IndexOutOfBoundsException("bitIndex < 0: " + bitIndex);
        }
        //Ѱ�Ҵ洢��bit�Ķ�Ӧ��word
        int wordIndex = wordIndex(bitIndex);

        this.words[wordIndex] ^= (1L << bitIndex);

    }

    public boolean empty() {
        return words.length == 0;
    }


    // �ж�����bitmap�Ƿ��н���
    public boolean intersects(BitMap b){
        if(b.empty())return false;
        for(int idx = Math.min(b.words.length, this.words.length) - 1; idx >=0; idx--) {
            if((b.words[idx] & this.words[idx]) != 0){
                return true;
            }
        }
        return false;
    }

    public int OneBitCount() {
        int sum = 0;
        for(int i = 0; i<words.length;i++){
            sum += Long.bitCount(words[i]);
        }
        return sum;
    }


    private void tryExpand(int wordIndex) {
        int wordRequired = wordIndex + 1;
        if(wordRequired<=words.length)return;
        int newLength = Math.max(2*words.length, wordRequired);
        words = Arrays.copyOf(words, newLength);
    }

    private int wordCount() {
        return this.words.length;
    }


    public static void main(String[] args) {


        //By default word size is 1.
        BitMap bitMap = new BitMap();

        //Test1 set & get
        for(int i = 0; i < 10; i++){
            bitMap.set(i);
        }
        for(int i = 0; i < 15; i++){
            System.out.println("i= " + i + " " + bitMap.get(i));
        }

        //Test2
        System.out.println(bitMap.OneBitCount());

        //Test3 interset
        BitMap bitMap1 = new BitMap();
        BitMap bitMap2 = new BitMap();

        for(int i = 0; i < 12; i++){
            bitMap1.set(i);
        }

        for(int i = 11; i < 20; i++) {
            bitMap2.set(i);
        }
        //true
        System.out.println(bitMap.intersects(bitMap1));
        //false
        System.out.println(bitMap.intersects(bitMap2));


        //Test4 flip
        bitMap.flip(5);
        System.out.println(bitMap.get(5)); //false

        //Test 5 tryExpand
        bitMap.set(100);
        System.out.println(bitMap.wordCount()); //2


        List<Integer> randomList = new ArrayList<>();
        Random random = new Random();
        for(int i = 0; i < 10000000; i++){
            randomList.add(random.nextInt(100000000)+1);
        }

        BitSet bitSet = new BitSet(100000001);

        List<Integer> notInRandomList = new ArrayList<>();
        for(int i = 0; i <= 10000000;i++){
            if(!bitSet.get(i)){
                notInRandomList.add(i);
            }
        }

    }

}
