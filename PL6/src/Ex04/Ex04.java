package Ex04;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class Request {
    private int passengers;
    private String pickupAddress;

    public Request(int passengers, String pickupAddress) {
        this.passengers = passengers;
        this.pickupAddress = pickupAddress;
    }

    public int getPassengers() {
        return passengers;
    }

    public String getPickupAddress() {
        return pickupAddress;
    }
}

class Taxi {
    private int maxSeats;
    private int availableSeats;

    public Taxi(int maxSeats) {
        this.maxSeats = maxSeats;
        this.availableSeats = maxSeats;
    }

    public boolean isAvailable() {
        return availableSeats > 0;
    }

    public void pickUpPassengers(Request request) {
        int passengersToPickUp = Math.min(availableSeats, request.getPassengers());
        availableSeats -= passengersToPickUp;

        System.out.println("Picked up " + passengersToPickUp + " passengers at " + request.getPickupAddress());

        if (passengersToPickUp < request.getPassengers()) {
            // Add remaining passengers to the waiting list for the next available taxi
            Request remainingRequest = new Request(request.getPassengers() - passengersToPickUp, request.getPickupAddress());
            TaxiCentral.addToWaitingList(remainingRequest);
        }
    }

    public void freeUpSeats(int seats) {
        availableSeats += seats;
    }
}

class TaxiCentral {
    private static Queue<Request> waitingList = new LinkedList<>();
    private static Lock lock = new ReentrantLock();
    private static Condition taxiAvailable = lock.newCondition();

    public static void addToWaitingList(Request request) {
        lock.lock();
        try {
            waitingList.add(request);
            taxiAvailable.signal(); // Notify a waiting taxi that a new request is available
        } finally {
            lock.unlock();
        }
    }

    public static void assignTaxi(Taxi taxi) {
        lock.lock();
        try {
            while (waitingList.isEmpty() || !taxi.isAvailable()) {
                try {
                    taxiAvailable.await(); // Wait until a request is available and a taxi is free
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

public class TaxiSimulation {
    public static void main(String[] args) {
        Taxi taxi1 = new Taxi(4);
        Taxi taxi2 = new Taxi(6);

        Thread taxiThread1 = new Thread(() -> {
            while (true) {
                TaxiCentral.assignTaxi(taxi1);
                try {
                    Thread.sleep(1000); // Pause before the next assignment
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                taxi1.freeUpSeats(taxi1.getMaxSeats()); // Simulate dropping off passengers and freeing up seats
            }
        });

        Thread taxiThread2 = new Thread(() -> {
            while (true) {
                TaxiCentral.assignTaxi(taxi2);
                try {
                    Thread.sleep(1000); // Pause before the next assignment
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                taxi2.freeUpSeats(taxi2.getMaxSeats()); // Simulate dropping off passengers and freeing up seats
            }
        });

        taxiThread1.start();
        taxiThread2.start();

        // Simulating incoming requests
        Request request1 = new Request(3, "123 Main St");
        Request request2 = new Request(5, "456 Elm St");
        Request request3 = new Request(2, "789 Oak St");
        Request request4 = new Request(4, "321 Pine St");

        TaxiCentral.addToWaitingList(request1);
        TaxiCentral.addToWaitingList(request2);
        TaxiCentral.addToWaitingList(request3);
        TaxiCentral.addToWaitingList(request4);
    }
}
