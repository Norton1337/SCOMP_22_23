package src.Ex01;

import java.util.LinkedList;
import java.util.Queue;

class OrderQueue {
    private final Queue<Order> queue;

    public OrderQueue() {
        this.queue = new LinkedList<>();
    }

    public synchronized void addOrder(Order order) {
        queue.add(order);
        notifyAll();
    }

    public synchronized Order removeOrder() throws InterruptedException {
        while (queue.isEmpty()) {
            wait();
        }
        return queue.poll();
    }
}