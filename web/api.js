/**
 * @brief routing page of the API
 * @author Doriath Arthus
 * @date 2022 - 2023
 */

"use strict";
const express = require("express");
const bodyParser = require("body-parser");
const path = require('path');
const fs = require('fs');
const config = require("./config");

const app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());



/* home page, should return a list of all nodes */
app.get(config.HOMEPAGE, (req, res) => {
    res.json({
        test : {
            verb : "POST",
            brief : "inform the server that you've connected"
        },
        name : {
            verb : "GET",
            brief : "return a json containing the author's name"
        }
    });
});

//load all api endpoints
const endpointDir = path.join(__dirname, 'endpoints');

//get all filenames in /api/endpoints, exclude folders and remove the extension
let filenames = fs.readdirSync(endpointDir, { withFileTypes: true })
                  .filter(dirent => dirent.isFile())
                  .map(dirent => dirent.name.split('.')[0]);

filenames.forEach((filename) => {

    app.use(config.HOMEPAGE + "/" + filename, require(path.join(endpointDir, filename)));
})

app.set('trust proxy', true);
app.listen(config.PORT, console.log(`[${new Date().toLocaleString('fr-FR')}] Server started on port ${config.PORT} in ${config.MODE} mode`));
