const char* DATES[] = {
    "2018-01-01", "2018-01-02", "2018-01-03", "2018-01-04", "2018-01-05",
    "2018-01-06", "2018-01-07", "2018-01-08", "2018-01-09", "2018-01-10",
    "2018-01-11", "2018-01-12", "2018-01-13", "2018-01-14", "2018-01-15",
    "2018-01-16", "2018-01-17", "2018-01-18", "2018-01-19", "2018-01-20",
    "2018-01-21", "2018-01-22", "2018-01-23", "2018-01-24", "2018-01-25",
    "2018-01-26", "2018-01-27", "2018-01-28", "2018-01-29", "2018-01-30",
    "2018-01-31", "2018-02-01", "2018-02-02", "2018-02-03", "2018-02-04",
    "2018-02-05", "2018-02-06", "2018-02-07", "2018-02-08", "2018-02-09",
    "2018-02-10", "2018-02-11", "2018-02-12", "2018-02-13", "2018-02-14",
    "2018-02-15", "2018-02-16", "2018-02-17", "2018-02-18", "2018-02-19",
    "2018-02-20", "2018-02-21", "2018-02-22", "2018-02-23", "2018-02-24",
    "2018-02-25", "2018-02-26", "2018-02-27", "2018-02-28", "2018-03-01",
    "2018-03-02", "2018-03-03", "2018-03-04", "2018-03-05", "2018-03-06",
    "2018-03-07", "2018-03-08", "2018-03-09", "2018-03-10", "2018-03-11",
    "2018-03-12", "2018-03-13", "2018-03-14", "2018-03-15", "2018-03-16",
    "2018-03-17", "2018-03-18", "2018-03-19", "2018-03-20", "2018-03-21",
    "2018-03-22", "2018-03-23", "2018-03-24", "2018-03-25", "2018-03-26",
    "2018-03-27", "2018-03-28", "2018-03-29", "2018-03-30", "2018-03-31",
    "2018-04-01", "2018-04-02", "2018-04-03", "2018-04-04", "2018-04-05",
    "2018-04-06", "2018-04-07", "2018-04-08", "2018-04-09", "2018-04-10",
    "2018-04-11", "2018-04-12", "2018-04-13", "2018-04-14", "2018-04-15",
    "2018-04-16", "2018-04-17", "2018-04-18", "2018-04-19", "2018-04-20",
    "2018-04-21", "2018-04-22", "2018-04-23", "2018-04-24", "2018-04-25",
    "2018-04-26", "2018-04-27", "2018-04-28", "2018-04-29", "2018-04-30",
    "2018-05-01", "2018-05-02", "2018-05-03", "2018-05-04", "2018-05-05",
    "2018-05-06", "2018-05-07", "2018-05-08", "2018-05-09", "2018-05-10",
    "2018-05-11", "2018-05-12", "2018-05-13", "2018-05-14", "2018-05-15",
    "2018-05-16", "2018-05-17", "2018-05-18", "2018-05-19", "2018-05-20",
    "2018-05-21", "2018-05-22", "2018-05-23", "2018-05-24", "2018-05-25",
    "2018-05-26", "2018-05-27", "2018-05-28", "2018-05-29", "2018-05-30",
    "2018-05-31", "2018-06-01", "2018-06-02", "2018-06-03", "2018-06-04",
    "2018-06-05", "2018-06-06", "2018-06-07", "2018-06-08", "2018-06-09",
    "2018-06-10", "2018-06-11", "2018-06-12", "2018-06-13", "2018-06-14",
    "2018-06-15", "2018-06-16", "2018-06-17", "2018-06-18", "2018-06-19",
    "2018-06-20", "2018-06-21", "2018-06-22", "2018-06-23", "2018-06-24",
    "2018-06-25", "2018-06-26", "2018-06-27", "2018-06-28", "2018-06-29",
    "2018-06-30", "2018-07-01", "2018-07-02", "2018-07-03", "2018-07-04",
    "2018-07-05", "2018-07-06", "2018-07-07", "2018-07-08", "2018-07-09",
    "2018-07-10", "2018-07-11", "2018-07-12", "2018-07-13", "2018-07-14",
    "2018-07-15", "2018-07-16", "2018-07-17", "2018-07-18", "2018-07-19",
    "2018-07-20", "2018-07-21", "2018-07-22", "2018-07-23", "2018-07-24",
    "2018-07-25", "2018-07-26", "2018-07-27", "2018-07-28", "2018-07-29",
    "2018-07-30", "2018-07-31", "2018-08-01", "2018-08-02", "2018-08-03",
    "2018-08-04", "2018-08-05", "2018-08-06", "2018-08-07", "2018-08-08",
    "2018-08-09", "2018-08-10", "2018-08-11", "2018-08-12", "2018-08-13",
    "2018-08-14", "2018-08-15", "2018-08-16", "2018-08-17", "2018-08-18",
    "2018-08-19", "2018-08-20", "2018-08-21", "2018-08-22", "2018-08-23",
    "2018-08-24", "2018-08-25", "2018-08-26", "2018-08-27", "2018-08-28",
    "2018-08-29", "2018-08-30", "2018-08-31", "2018-09-01", "2018-09-02",
    "2018-09-03", "2018-09-04", "2018-09-05", "2018-09-06", "2018-09-07",
    "2018-09-08", "2018-09-09", "2018-09-10", "2018-09-11", "2018-09-12",
    "2018-09-13", "2018-09-14", "2018-09-15", "2018-09-16", "2018-09-17",
    "2018-09-18", "2018-09-19", "2018-09-20", "2018-09-21", "2018-09-22",
    "2018-09-23", "2018-09-24", "2018-09-25", "2018-09-26", "2018-09-27",
    "2018-09-28", "2018-09-29", "2018-09-30", "2018-10-01", "2018-10-02",
    "2018-10-03", "2018-10-04", "2018-10-05", "2018-10-06", "2018-10-07",
    "2018-10-08", "2018-10-09", "2018-10-10", "2018-10-11", "2018-10-12",
    "2018-10-13", "2018-10-14", "2018-10-15", "2018-10-16", "2018-10-17",
    "2018-10-18", "2018-10-19", "2018-10-20", "2018-10-21", "2018-10-22",
    "2018-10-23", "2018-10-24", "2018-10-25", "2018-10-26", "2018-10-27",
    "2018-10-28", "2018-10-29", "2018-10-30", "2018-10-31", "2018-11-01",
    "2018-11-02", "2018-11-03", "2018-11-04", "2018-11-05", "2018-11-06",
    "2018-11-07", "2018-11-08", "2018-11-09", "2018-11-10", "2018-11-11",
    "2018-11-12", "2018-11-13", "2018-11-14", "2018-11-15", "2018-11-16",
    "2018-11-17", "2018-11-18", "2018-11-19", "2018-11-20", "2018-11-21",
    "2018-11-22", "2018-11-23", "2018-11-24", "2018-11-25", "2018-11-26",
    "2018-11-27", "2018-11-28", "2018-11-29", "2018-11-30", "2018-12-01",
    "2018-12-02", "2018-12-03", "2018-12-04", "2018-12-05", "2018-12-06",
    "2018-12-07", "2018-12-08", "2018-12-09", "2018-12-10", "2018-12-11",
    "2018-12-12", "2018-12-13", "2018-12-14", "2018-12-15", "2018-12-16",
    "2018-12-17", "2018-12-18", "2018-12-19", "2018-12-20", "2018-12-21",
    "2018-12-22", "2018-12-23", "2018-12-24", "2018-12-25", "2018-12-26",
    "2018-12-27", "2018-12-28", "2018-12-29", "2018-12-30", "2018-12-31",
};
