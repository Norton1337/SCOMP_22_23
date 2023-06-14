package src.Ex01;

class OrderTaker implements Runnable {
    private final OrderQueue orderQueue;
    private static int nextOrderId = 1;
    private final int simulationTime;

    public OrderTaker(OrderQueue orderQueue, int simulationTime) {
        this.orderQueue = orderQueue;
        this.simulationTime = simulationTime;
    }

    @Override
    public void run() {
        try {
            while (true) {
                Order order = new Order(nextOrderId++);
                orderQueue.addOrder(order);
                System.out.println("Order added: " + order.orderId());
                Thread.sleep(simulationTime);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
