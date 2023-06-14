package src.Ex04;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class TaxiCentral {
    private static final Queue<Request> waitingList = new LinkedList<>();
    private static final Lock lock = new ReentrantLock();
    private static final Condition taxiAvailable = lock.newCondition();

    public static void addToWaitingList(Request request) {
        lock.lock();
        try {
            waitingList.add(request);
            taxiAvailable.signal();
        } finally {
            lock.unlock();
        }
    }

    public static void assignTaxi(Taxi taxi) {
        lock.lock();
        try {
            while (waitingList.isEmpty() || !taxi.isAvailable()) {
                try {
                    taxiAvailable.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            Request request = waitingList.poll();
            taxi.pickUpPassengers(request);
        } finally {
            lock.unlock();
        }
    }
}
