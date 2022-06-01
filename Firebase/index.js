const functions = require("firebase-functions");
var admin = require("firebase-admin");

admin.initializeApp(functions.config().firebase);
var database = admin.database();

exports.controlVirtualLedFunction = functions.https.onRequest(async(req, res) => {
    let params = req.body.queryResult.parameters;
    await database.ref("<Parent database adress for parameters>").update(params);
    res.send("Okay, led controlled successfully");
});
