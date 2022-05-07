import 'dart:developer';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
// import 'package:medicine_reminder/main.dart';
// import 'daysScreen.dart';

class daySevenDose extends StatefulWidget {
  const daySevenDose({Key? key}) : super(key: key);

  @override
  _daySevenDoseState createState() => _daySevenDoseState();
}

class _daySevenDoseState extends State<daySevenDose> {
  TimeOfDay _timeOfDayOne = TimeOfDay(hour: 8, minute: 30);
  TimeOfDay _timeOfDayTwo = TimeOfDay(hour: 8, minute: 30);
  TimeOfDay _timeOfDayThree = TimeOfDay(hour: 8, minute: 30);
  // ignore: deprecated_member_use
  final dbref = FirebaseDatabase.instance.reference();

  late int doseOneHour,
      doseOneMinute,
      doseTwoHour,
      doseTwoMinute,
      doseThreeHour,
      doseThreeMinute;

  // show time picker method
  void _showTimePickerOne() {
    showTimePicker(
      context: context,
      initialTime: TimeOfDay.now(),
    ).then((value) {
      setState(() {
        _timeOfDayOne = value!;
      });
    });
  }

  void _showTimePickerTwo() {
    showTimePicker(
      context: context,
      initialTime: TimeOfDay.now(),
    ).then((value) {
      setState(() {
        _timeOfDayTwo = value!;
      });
    });
  }

  void _showTimePickerThree() {
    showTimePicker(
      context: context,
      initialTime: TimeOfDay.now(),
    ).then((value) {
      setState(() {
        _timeOfDayThree = value!;
      });
    });
  }

  void _saveButton() {
    // doseOnehour = _timeOfDayOne.hour;
    // log('$doseOnehour');
    // log(_timeOfDayTwo.format(context).toString());
    // log(_timeOfDayThree.format(context).toString());

    // // dbref.update({'led_one': 0, 'led_two': 0});
    // dbref.update({'monday/dose_1': _timeOfDayOne.format(context).toString()});
    // dbref.update({'monday/dose_2': _timeOfDayTwo.format(context).toString()});
    // dbref.update({'monday/dose_3': _timeOfDayThree.format(context).toString()});
    doseOneHour = _timeOfDayOne.hour;
    doseOneMinute = _timeOfDayOne.minute;

    doseTwoHour = _timeOfDayTwo.hour;
    doseTwoMinute = _timeOfDayTwo.minute;

    doseThreeHour = _timeOfDayThree.hour;
    doseThreeMinute = _timeOfDayThree.minute;

    dbref.update({'saturday/dose_1Hour': doseOneHour});
    dbref.update({'saturday/dose_1Minute': doseOneMinute});

    dbref.update({'saturday/dose_2Hour': doseTwoHour});
    dbref.update({'saturday/dose_2Minute': doseTwoMinute});

    dbref.update({'saturday/dose_3Hour': doseThreeHour});
    dbref.update({'saturday/dose_3Minute': doseThreeMinute});

    dbref.update({'flagStatus': 0});
  }

  @override
  Widget build(BuildContext context) => Scaffold(
      appBar: AppBar(
        title: Text('Saturday'),
        centerTitle: true,
      ),
      body: Column(
        children: <Widget>[
          Expanded(
              child: Container(
            alignment: Alignment.topLeft,
            padding: EdgeInsets.all(20),
            // color: Colors.blue,
            child: Text(
              _timeOfDayOne.format(context).toString(),
              style: TextStyle(
                color: Colors.black,
                fontSize: 35,
                fontWeight: FontWeight.bold,
              ),
            ),
          )),
          MaterialButton(
            onPressed: _showTimePickerOne,
            child: const Padding(
              padding: EdgeInsets.all(20),
              child: Text(
                'Pick Time',
                style: TextStyle(color: Colors.blue, fontSize: 30),
              ),
            ),
          ),
          Expanded(
              child: Container(
            alignment: Alignment.topLeft,
            padding: EdgeInsets.all(20),
            // color: Colors.blue,
            child: Text(
              _timeOfDayTwo.format(context).toString(),
              style: TextStyle(
                color: Colors.black,
                fontSize: 35,
                fontWeight: FontWeight.bold,
              ),
            ),
          )),
          Expanded(
              child: MaterialButton(
            onPressed: _showTimePickerTwo,
            child: const Padding(
              padding: EdgeInsets.all(20),
              child: Text(
                'Pick Time',
                style: TextStyle(color: Colors.blue, fontSize: 30),
              ),
            ),
          )),
          Expanded(
              child: Container(
            alignment: Alignment.topLeft,
            padding: EdgeInsets.all(20),
            // color: Colors.blue,
            child: Text(
              _timeOfDayThree.format(context).toString(),
              style: TextStyle(
                color: Colors.black,
                fontSize: 35,
                fontWeight: FontWeight.bold,
              ),
            ),
          )),
          Expanded(
              child: MaterialButton(
            onPressed: _showTimePickerThree,
            child: const Padding(
              padding: EdgeInsets.all(20),
              child: Text(
                'Pick Time',
                style: TextStyle(color: Colors.blue, fontSize: 30),
              ),
            ),
          )),
          Expanded(
            child: MaterialButton(
              child: Text(
                'Click to Save',
                style: TextStyle(fontSize: 24),
              ),
              onPressed: _saveButton,
            ),
          )
        ],
      ));
}
