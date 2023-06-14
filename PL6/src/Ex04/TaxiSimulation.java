package src.Ex04;


public class TaxiSimulation {
    public static void main(String[] args) {
        Taxi taxi1 = new Taxi(4);
        Taxi taxi2 = new Taxi(6);

        Thread taxiThread1 = new Thread(() -> {
            while (true) {
                TaxiCentral.assignTaxi(taxi1);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                taxi1.freeUpSeats(taxi1.getMaxSeats());
            }
        });

        Thread taxiThread2 = new Thread(() -> {
            while (true) {
                TaxiCentral.assignTaxi(taxi2);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                taxi2.freeUpSeats(taxi2.getMaxSeats());
            }
        });

        taxiThread1.start();
        taxiThread2.start();

        Request request1 = new Request(3, "São Pedro Fins");
        Request request2 = new Request(5, "Santo Ovido");
        Request request3 = new Request(2, "Castêlo da Maia");
        Request request4 = new Request(4, "Paços do Brandão");
        Request request5 = new Request(61, "Travagem");

        TaxiCentral.addToWaitingList(request1);
        TaxiCentral.addToWaitingList(request2);
        TaxiCentral.addToWaitingList(request3);
        TaxiCentral.addToWaitingList(request4);
        TaxiCentral.addToWaitingList(request5);

    }
}
