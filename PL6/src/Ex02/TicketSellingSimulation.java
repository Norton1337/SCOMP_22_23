package src.Ex02;

import java.util.concurrent.Semaphore;

public class TicketSellingSimulation {
    private static final int NUM_SELLERS = 5;
    private static final int NUM_TICKETS = 20;

    private static Semaphore semaphore = new Semaphore(1);

    private static int availableTickets = NUM_TICKETS;

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
                    semaphore.acquire();
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
                    semaphore.release();
                }

                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
