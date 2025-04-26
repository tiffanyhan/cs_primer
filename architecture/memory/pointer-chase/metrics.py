import csv
import math
import time


def average_age(ages):
    total = 0
    for age in ages:
        total += age
    return total / len(ages)


def average_payment_amount(payments):
    amount = 0
    for p in payments:
        amount += p
    return amount / len(payments)


def stddev_payment_amount(payments, mean):
    squared_diffs = 0
    for p in payments:
        diff = p - mean
        squared_diffs += diff * diff
    return math.sqrt(squared_diffs / len(payments))


def load_data():
    ages, payments = [], []
    with open('users.csv') as f:
        for line in csv.reader(f):
            _, _, age, _, _ = line
            ages.append(int(age))
    with open('payments.csv') as f:
        for line in csv.reader(f):
            amount_cents, _, _ = line
            amount = int(amount_cents) / 100
            payments.append(amount)
    return ages, payments


# Data loading: 0.398s
# Computation 0.037s
if __name__ == '__main__':
    t = time.perf_counter()
    ages, payments = load_data()
    print(f'Data loading: {time.perf_counter() - t:.3f}s')
    t = time.perf_counter()
    assert abs(average_age(ages) - 59.626) < 0.01
    payments_mean = average_payment_amount(payments)
    assert abs(payments_mean - 499850.559) < 0.01
    assert abs(stddev_payment_amount(payments, payments_mean) - 288684.849) < 0.01
    print(f'Computation {time.perf_counter() - t:.3f}s')
