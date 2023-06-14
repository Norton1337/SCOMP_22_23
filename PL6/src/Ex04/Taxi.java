package src.Ex04;
public class Taxi {
    private final int maxSeats;
    private int availableSeats;

    public Taxi(int maxSeats) {
        this.maxSeats = maxSeats;
        this.availableSeats = maxSeats;
    }

    public boolean isAvailable() {
        return availableSeats > 0;
    }

    public void pickUpPassengers(Request request) {
        int passengersToPickUp = Math.min(availableSeats, request.passengers());
        availableSeats -= passengersToPickUp;

        System.out.println("Picked up " + passengersToPickUp + " passengers at " + request.pickupAddress());

        if (passengersToPickUp < request.passengers()) {
            Request remainingRequest = new Request(request.passengers() - passengersToPickUp, request.pickupAddress());
            TaxiCentral.addToWaitingList(remainingRequest);
        }
    }

    public void freeUpSeats(int seats) {
        availableSeats += seats;
    }

    public int getMaxSeats() {
        return maxSeats;
    }
}


