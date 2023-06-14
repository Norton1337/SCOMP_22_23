package src.Ex03;

class BankTransfer {
    public static boolean transferMoney(Account payerAccount, Account payeeAccount, int amount) {
        Account firstAccount;
        Account secondAcount;

        if (payerAccount.hashCode() < payeeAccount.hashCode()) {
            firstAccount = payerAccount;
            secondAcount = payeeAccount;
        } else {
            firstAccount = payeeAccount;
            secondAcount = payerAccount;
        }

        firstAccount.lock.lock();
        try {
            secondAcount.lock.lock();
            try {
                if (payerAccount.getBalance() < amount) {
                    return false;
                }

                payerAccount.withdraw(amount);
                payeeAccount.deposit(amount);

                return true;
            } finally {
                secondAcount.lock.unlock();
            }
        } finally {
            firstAccount.lock.unlock();
        }
    }
}