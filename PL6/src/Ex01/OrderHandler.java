package src.Ex01;

class OrderHandler implements Runnable {
    private final OrderQueue orderQueue;

    private final int simulationTime;

    public OrderHandler(OrderQueue orderQueue, int simulationTime) {
        this.orderQueue = orderQueue;
        this.simulationTime = simulationTime;
    }

    @Override
    public void run() {
        try {
            while (true) {
                Order order = orderQueue.removeOrder();
                System.out.println("Order handled: " + order.orderId());
                Thread.sleep(simulationTime);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}