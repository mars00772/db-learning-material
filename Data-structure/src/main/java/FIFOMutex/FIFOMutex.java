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

        //当前线程入队
        waitingQueue.add(Thread.currentThread());

        //加入当前线程不是队头或者设置lock失败，即上一个获得锁的线程还没解锁成功 就park阻塞线程
        while(waitingQueue.peek()!=Thread.currentThread()||!locked.compareAndSet(false, true)){
            LockSupport.park(this);
            //要考虑中断的问题
            if(Thread.interrupted()){
                isInterrupted = true;
            }
        }

        //移除队头
        waitingQueue.remove();
        if(isInterrupted){
            //设置中断
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
