from datetime import date, timedelta

t = date(2018, 1, 1)
end = date(2018, 12, 31)

print("const char* DATES[] = {")
while t <= end:
    print(f'"{t.strftime("%Y-%m-%d")}",')
    t += timedelta(days=1)

print("};")
