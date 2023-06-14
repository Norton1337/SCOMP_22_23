package src.Ex01;

public class Main {
    public static void main(String[] args) {
        OrderQueue orderQueue = new OrderQueue();
        int numOrderTakers = 3;
        int numOrderHandlers = 2;

        for (int i = 0; i < numOrderTakers; i++) {
            OrderTaker orderTaker = new OrderTaker(orderQueue, 2000);
            Thread orderTakerThread = new Thread(orderTaker);
            orderTakerThread.start();
        }

        for (int i = 0; i < numOrderHandlers; i++) {
            OrderHandler orderHandler = new OrderHandler(orderQueue, 1000);
            Thread orderHandlerThread = new Thread(orderHandler);
            orderHandlerThread.start();
        }
    }
}
