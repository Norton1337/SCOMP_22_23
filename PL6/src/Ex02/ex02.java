import java.util.concurrent.Semaphore;

public class TicketSellingSimulation {
    private static final int NUM_SELLERS = 5; // Number of ticket sellers
    private static final int NUM_TICKETS = 20; // Total number of tickets available

    private static Semaphore semaphore = new Semaphore(1); // Semaphore for access to shared variable

    private static int availableTickets = NUM_TICKETS; // Shared variable for available tickets

    public static void main(String[] args) {
        for (int i = 1; i <= NUM_SELLERS; i++) {
            Seller seller = new Seller(i);
            seller.start();
        }
    }

    static class Seller extends Thread {
        private int sellerId;

        public Seller(int sellerId) {
            this.sellerId = sellerId;
        }

        public void run() {
            while (true) {
                try {
                    semaphore.acquire(); // Acquire semaphore to access shared variable
                    if (availableTickets > 0) {
                        System.out.println("Seller " + sellerId + " sold ticket " + availableTickets);
                        availableTickets--;
                    } else {
                        System.out.println("Seller " + sellerId + " sold out");
                        break;
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    semaphore.release(); // Release semaphore
                }

                try {
                    Thread.sleep(1000); // Pause for a while before selling the next ticket
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
