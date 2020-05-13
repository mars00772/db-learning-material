package FIFOMutex;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedDeque;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.LockSupport;

public class FIFOMutex {

    static {
        Class<?> ensuredLoaded = LockSupport.class;
    }

    private final AtomicBoolean locked = new AtomicBoolean(false);

    private final Queue<Thread> waitingQueue = new ConcurrentLinkedDeque<>();

    public void lock(){

        boolean isInterrupted = false;

        //��ǰ�߳����
        waitingQueue.add(Thread.currentThread());

        //���뵱ǰ�̲߳��Ƕ�ͷ��������lockʧ�ܣ�����һ����������̻߳�û�����ɹ� ��park�����߳�
        while(waitingQueue.peek()!=Thread.currentThread()||!locked.compareAndSet(false, true)){
            LockSupport.park(this);
            //Ҫ�����жϵ�����
            if(Thread.interrupted()){
                isInterrupted = true;
            }
        }

        //�Ƴ���ͷ
        waitingQueue.remove();
        if(isInterrupted){
            //�����ж�
            Thread.currentThread().interrupt();
        }

    }

    public void unlock(){
        locked.set(false);
        LockSupport.unpark(waitingQueue.peek());
    }

    public static void main(String[] args) throws Exception {

        FIFOMutex fifoMutex = new FIFOMutex();
        Thread t1 = new Thread(new MyThread("t1", fifoMutex));
        Thread t2 = new Thread(new MyThread("t2", fifoMutex));
        Thread t3 = new Thread(new MyThread("t3", fifoMutex));

        t1.start();
        t2.start();
        t3.start();

        t1.join();
        t2.join();
        t3.join();

        assert MyThread.count == 300;

    }
}

class MyThread implements Runnable {
    private String name;
    private FIFOMutex fifoMutex;
    public static int count = 0;

    MyThread(String name, FIFOMutex fifoMutex){
        this.fifoMutex  =fifoMutex;
        this.name=name;
    }

    @Override
    public void run(){
        for(int i = 0; i<100;i++){
            fifoMutex.lock();
            System.out.println(this.name + " : " + ++count);
            fifoMutex.unlock();
        }
    }
}
