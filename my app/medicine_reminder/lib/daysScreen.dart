import 'dart:async';
import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:medicine_reminder/dayFiveDose.dart';
import 'package:medicine_reminder/dayFourDose.dart';
import 'package:medicine_reminder/daySevenDose.dart';
import 'package:medicine_reminder/daySixDose.dart';
import 'package:medicine_reminder/dayThreeDose.dart';
import 'package:medicine_reminder/dayTwoDose.dart';
import 'main.dart';
import 'dayOneDose.dart';

class daysScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) => Scaffold(
      appBar: AppBar(
        title: Text('First Screen'),
        centerTitle: true,
      ),
      body: Center(
        child: SingleChildScrollView(
          clipBehavior: Clip.antiAlias,
          child: Column(
            children: [
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Sunday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => dayOneDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Monday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => dayTwoDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Tuesday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => dayThreeDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Wednesday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => dayFourDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Thursday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => dayFiveDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Friday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => daySixDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
              ListTile(
                leading: Icon(Icons.arrow_drop_down_circle),
                title: const Text('Saturday'),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 1',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 2',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Text(
                  'Dose 3',
                  style: TextStyle(color: Colors.black.withOpacity(0.6)),
                ),
              ),
              ButtonBar(
                alignment: MainAxisAlignment.start,
                children: [
                  FlatButton(
                    textColor: const Color(0xFF6200EE),
                    onPressed: () {
                      // Perform some action
                      Navigator.push(
                          context,
                          MaterialPageRoute(
                              builder: (context) => daySevenDose()));
                    },
                    child: const Text('Pick Time'),
                  ),
                ],
              ),
            ],
          ),
        ),
      ));
}
