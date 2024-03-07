import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';

const jsonFileURL = '../simulation/proton.json';

let camera, scene, renderer;
const planets = [];
const satellites = [];
const satelliteColor = [];

let trails = []
let trails_sat = []
let jsonData;
let controls;

const planetColors = [
    0xff0000, // Red
    0xffff00, // Yellow
    0x008000, // Green
    0xffa500, // Orange
    0x0000ff, // Blue
    0x4b0082, // Indigo
    0x9400d3, // Violet
    0xffc0cb, // Pink
  ];

let timeIndex = 0;

const legendElement = document.getElementById('legend');

function createTrail(color, startpos) {
    const material = new THREE.LineBasicMaterial({ color: color });
    const geometry = new THREE.BufferGeometry().setFromPoints([startpos]);
    const trail = new THREE.Line(geometry, material);
    scene.add(trail);

    return { object: trail, points: [startpos] }
}

function initializeTrails() {
    const numSatellites = jsonData.simulation.time_steps[0].satellites.length;

    for (let i = 0; i < numSatellites; i++) {
        const randomColor = 0xffffff;
        satelliteColor.push(randomColor)
        let spos = new THREE.Vector3(jsonData.simulation.time_steps[0].satellites[i].x * 1e-10, jsonData.simulation.time_steps[0].satellites[i].y  * 1e-10, 
            jsonData.simulation.time_steps[0].satellites[i].z  * 1e-10)
        trails_sat.push(createTrail(randomColor, spos));
    }

    const numPlanets = jsonData.simulation.time_steps[0].planets.length;
    for (let i = 0; i < numPlanets; i++) {
        const color = planetColors[i];
        let spos = new THREE.Vector3(jsonData.simulation.time_steps[0].planets[i].x * 1e-10, jsonData.simulation.time_steps[0].planets[i].y  * 1e-10, 
            jsonData.simulation.time_steps[0].planets[i].z  * 1e-10)
        trails.push(createTrail(color, spos)); 
    }
}

function init() {
    // Set up the scene, camera, and renderer
    camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
    );
    camera.position.z = 5;

    scene = new THREE.Scene();

    renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    // controls

    controls = new OrbitControls( camera, renderer.domElement );
    controls.listenToKeyEvents( window ); // optional

    controls.enableDamping = true;
    controls.enableZoom = true;
    controls.dampingFactor = 0.25;
    controls.screenSpacePanning = false;
    controls.maxPolarAngle = Math.PI / 2;

    // Adjust Camera Position to Zoom
    camera.position.z = 100;

    const planetGeometry = new THREE.SphereGeometry(0.2, 32, 32);
    const sunMaterial = new THREE.MeshBasicMaterial({
    color: 0xFFCC33,
    });
    const sun = new THREE.Mesh(
        planetGeometry,
        sunMaterial
    );
    sun.position.set(
        0,
        0,
        0
    );

    scene.add(sun);


    loadJSON(jsonFileURL)
    .then(data => {
        jsonData = data;
        jsonData.simulation.time_steps[0].planets.forEach(
        (planetData, index) => {
            const color = new THREE.Color(planetColors[index]);
            const planetName = jsonData.simulation.time_steps[timeIndex].planets[index].name;

            const planetGeometry = new THREE.SphereGeometry(0.2, 32, 32);

        
            const planet = new THREE.Mesh(
                planetGeometry
            );
            planet.position.set(
                planetData.x * 1e-10,
                planetData.y * 1e-10,
                planetData.z * 1e-10
            );

            planet.material.color = color;

            //console.log(planetData.x, planetData.x);
            planets.push(planet);
            scene.add(planet);
                
            legendElement.innerHTML += `<div><span style="color: #${color.getHexString()};">${planetName}</span></div>`;
        }
        );

        jsonData.simulation.time_steps[0].satellites.forEach(
            (satelliteData, index) => {
                const color = new THREE.Color(satelliteColor[index]);
                const satellitesName = jsonData.simulation.time_steps[timeIndex].satellites[index].name;
    
                const satellitesGeometry = new THREE.SphereGeometry(0.2, 32, 32);
    
            
                const satellite = new THREE.Mesh(
                    satellitesGeometry
                );
                satellite.position.set(
                    satelliteData.x * 1e-10,
                    satelliteData.y * 1e-10,
                    satelliteData.z * 1e-10
                );
    
                satellite.material.color = color;
    
                //console.log(planetData.x, planetData.x);
                satellites.push(satellite);
                scene.add(satellite);
                    
                legendElement.innerHTML += `<div><span style="color: #${color.getHexString()};">${satellitesName}</span></div>`;
            }
            );

        // Start the animation loop
        initializeTrails();
        animate();
        })
        .catch(error => {
        console.error('Error loading JSON:', error);
        });
}

function loadJSON(jsonFileURL) {
    // Implement your JSON loading logic here
    // You can use fetch or any other method to load the JSON data
    return fetch(jsonFileURL).then(response => response.json());
}

function animate() {

    planets.forEach((planet, index) => {
        const planetData = jsonData.simulation.time_steps[timeIndex].planets[index];
        planet.position.set(
            planetData.x * 1e-10,
            planetData.y * 1e-10,
            planetData.z * 1e-10,
        );

        //console.log("update", planet.position)

        trails[index].points.push(planet.position.clone());

        trails[index].object.geometry.setFromPoints(trails[index].points);
    });

    satellites.forEach((satellite, index) => {
        const satelliteData = jsonData.simulation.time_steps[timeIndex].satellites[index];
        satellite.position.set(
            satelliteData.x * 1e-10,
            satelliteData.y * 1e-10,
            satelliteData.z * 1e-10,
        );

        trails_sat[index].points.push(satellite.position.clone());

        trails_sat[index].object.geometry.setFromPoints(trails_sat[index].points);
    });
    // Render the scene
    renderer.render(scene, camera);
    
    controls.update();
    timeIndex += 1;

    if (timeIndex < jsonData.simulation.time_steps.length) {
        // Use setTimeout to introduce a delay (e.g., 100 milliseconds)
        setTimeout(() => {
            // Call animate again on the next frame
            requestAnimationFrame(animate);
        }, 100); // Adjust the delay as needed
    }
}

init();