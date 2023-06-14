package src.Ex03;

public class BankTransferSimulation {
    public static void main(String[] args) {
        Account account1 = new Account(1000);
        Account account2 = new Account(2000);

        boolean success = BankTransfer.transferMoney(account1, account2, 500);

        if (success) {
            System.out.println("Transfer successful!");
        } else {
            System.out.println("Insufficient funds!");
        }

        System.out.println("Account 1 balance: " + account1.getBalance());
        System.out.println("Account 2 balance: " + account2.getBalance());
    }
}