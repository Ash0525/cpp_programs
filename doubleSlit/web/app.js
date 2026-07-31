// Draw the barrier, slit openings, and viewing screen.
function drawWaveGeometry(visualState) {
    const canvas = document.getElementById("waveCanvas");
    const context = canvas.getContext("2d");

    const barrierX = visualState?.barrierX ?? 220;
    const screenX = visualState?.screenX ?? 820;
    const centerY = canvas.height / 2;
    const slitSeparation =
        visualState?.slitSeparationPixels ?? 90;
    const slitHeight =
        visualState?.slitHeightPixels ?? 28;

    // Controls the slits
    const upperSlitY = centerY - slitSeparation / 2;
    const lowerSlitY = centerY + slitSeparation / 2;

    // Clear the previous frame and configure the geometry lines.
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.strokeStyle = "#d4d4d8";
    context.lineWidth = 5;

    // Draw three barrier segments, leaving an opening at each slit.
    context.beginPath();

    context.moveTo(barrierX, 0);
    context.lineTo(barrierX, upperSlitY - slitHeight / 2);

    context.moveTo(barrierX, upperSlitY + slitHeight / 2);
    context.lineTo(barrierX, lowerSlitY - slitHeight / 2);

    context.moveTo(barrierX, lowerSlitY + slitHeight / 2);
    context.lineTo(barrierX, canvas.height);

    context.stroke();

    // Draw the viewing screen.
    context.beginPath();
    context.moveTo(screenX, 20);
    context.lineTo(screenX, canvas.height - 20);
    context.stroke();

    // Label the experiment components.
    context.fillStyle = "#e4e4e7";
    context.font = "16px sans-serif";
    context.textAlign = "center";
    context.fillText("Double slits", barrierX, 18);
    context.fillText("Screen", screenX, 18);

    // Canvas drawing context, barrier and screen position, upper
    // and lower slit positions
    return {
        canvas,
        context,
        barrierX,
        screenX,
        upperSlitY,
        lowerSlitY,

        wavefrontSpacingPixels:
            visualState?.wavefrontSpacingPixels ?? 36,

        waveHue:
            visualState?.waveHue ?? 220
    };
}

// Draw incoming plane waves.
function drawIncomingWavefronts(scene, time) {
    const context = scene.context;
    const spacing = scene.wavefrontSpacingPixels;
    const visualSpeed = 70;

    const movingOffset = (time * visualSpeed) % spacing;

    const firstWaveX = 20 + movingOffset;
    const finalWaveX = scene.barrierX - 10;

    context.save();

    context.strokeStyle =
        `hsl(${scene.waveHue} 95% 72% / 1)`;

    context.lineWidth = 2.5;

    for (
        let waveX = firstWaveX;
        waveX < finalWaveX;
        waveX += spacing
    ) {
        context.beginPath();
        context.moveTo(waveX, 30);
        context.lineTo(
            waveX,
            scene.canvas.height - 30
        );
        context.stroke();
    }

    context.restore();
}

// Draw the C++-generated intensity values on the graph canvas.
function drawIntensityGraph(positions, intensities) {
    const canvas = document.getElementById("intensityGraph");
    const context = canvas.getContext("2d");

    const width = canvas.width;
    const height = canvas.height;
    const padding = 60;
    const sampleCount = positions.size();
    const minPosition = positions.get(0);
    const maxPosition = positions.get(sampleCount - 1);

    // Clear any graph left over from a previous draw.
    context.clearRect(0, 0, width, height);

    // Draw the vertical intensity axis and horizontal position axis.
    context.strokeStyle = "#71717a";
    context.lineWidth = 1;
    context.beginPath();
    context.moveTo(padding, padding);
    context.lineTo(padding, height - padding);
    context.lineTo(width - padding, height - padding);
    context.stroke();

    // Draw axis values and labels
    context.fillStyle = "#e4e4e7";
    context.font = "14px sans-serif";

    // Horizontal position values, converted from meters to centimeters.
    context.textAlign = "center";
    context.fillText(
        `${(minPosition * 100).toFixed(1)} cm`,
        padding,
        height - padding + 22
    );
    context.fillText("0 cm", width / 2, height - padding + 22);
    context.fillText(
        `${(maxPosition * 100).toFixed(1)} cm`,
        width - padding,
        height - padding + 22
    );
    context.fillText(
        "Screen position",
        width / 2,
        height - 12
    );

    // Vertical intensity values.
    context.textAlign = "right";
    context.fillText("1.0", padding - 10, padding + 5);
    context.fillText("0.5", padding - 10, height / 2 + 5);
    context.fillText("0.0", padding - 10, height - padding + 5);

    // Rotated vertical-axis title.
    context.save();
    context.translate(18, height / 2);
    context.rotate(-Math.PI / 2);
    context.textAlign = "center";
    context.fillText("Normalized intensity", 0, 0);
    context.restore();

    // Draw the normalized intensity curve.
    context.strokeStyle = "#2563eb";
    context.lineWidth = 2;
    context.beginPath();

    for (let index = 0; index < sampleCount; index++) {
        const position = positions.get(index);
        const intensity = intensities.get(index);

        // Map the physical screen position across the drawable canvas width.
        const x =
            padding +
            ((position - minPosition) / (maxPosition - minPosition)) *
                (width - 2 * padding);

        // Canvas y-coordinates grow downward, so invert the intensity.
        const y =
            height -
            padding -
            intensity * (height - 2 * padding);

        if (index === 0) {
            context.moveTo(x, y);
        } else {
            context.lineTo(x, y);
        }
    }

    context.stroke();
}

// Map a value between numeric ranges.
function mapRange(
    value,
    inputMin,
    inputMax,
    outputMin,
    outputMax
) {
    // Scale physical values for the canvas.
    const ratio =
        (value - inputMin) / (inputMax - inputMin);

    return outputMin +
        ratio * (outputMax - outputMin);
}

// Read physical values from page controls
function readPhysicalState(controls) {
    const wavelengthNanometers =
        Number(controls.wavelengthSlider.value);

    const slitSeparationMicrometers =
        Number(controls.slitSeparationSlider.value);

    const slitWidthMicrometers =
        Number(controls.slitWidthSlider.value);

    const screenDistanceMeters =
        Number(controls.screenDistanceSlider.value);

    controls.wavelengthValue.textContent =
        `${wavelengthNanometers} nm`;

    controls.slitSeparationValue.textContent =
        `${slitSeparationMicrometers} µm`;

    controls.slitWidthValue.textContent =
        `${slitWidthMicrometers} µm`;

    controls.screenDistanceValue.textContent =
        `${screenDistanceMeters.toFixed(1)} m`;

    return {
        wavelength: wavelengthNanometers * 1e-9,
        slitSeparation:
            slitSeparationMicrometers * 1e-6,
        slitWidth: slitWidthMicrometers * 1e-6,
        screenDistance: screenDistanceMeters,
        diffractionEnabled:
            controls.diffractionToggle.checked
    };
}

// Send physical values into engine
function applyPhysicalState(engine, physicalState) {
    engine.setWavelength(physicalState.wavelength);

    engine.setSlitSeparation(
        physicalState.slitSeparation
    );

    engine.setSlitWidth(physicalState.slitWidth);

    engine.setScreenDistance(
        physicalState.screenDistance
    );

    engine.setDiffractionEnabled(
        physicalState.diffractionEnabled
    );
}

// Scale physical values for canvas visibility.
function createVisualState(physicalState, canvas) {
    const wavelengthNanometers =
        physicalState.wavelength * 1e9;

    const slitSeparationMicrometers =
        physicalState.slitSeparation * 1e6;

    const slitWidthMicrometers =
        physicalState.slitWidth * 1e6;

    return {
        barrierX: 170,

        screenX: mapRange(
            physicalState.screenDistance,
            0.5,
            2.0,
            560,
            canvas.width - 70
        ),

        slitSeparationPixels: mapRange(
            slitSeparationMicrometers,
            100,
            500,
            35,
            135
        ),

        slitHeightPixels: mapRange(
            slitWidthMicrometers,
            10,
            80,
            6,
            28
        ),

        wavefrontSpacingPixels: mapRange(
            wavelengthNanometers,
            380,
            750,
            22,
            46
        ),

        waveHue: mapRange(
            wavelengthNanometers,
            380,
            750,
            270,
            0
        )
    };
}

// Collect all interactive controls from the page.
function getSimulationControls() {
    return {
        diffractionToggle:
            document.getElementById("diffractionToggle"),
        wavelengthSlider:
            document.getElementById("wavelengthSlider"),
        wavelengthValue:
            document.getElementById("wavelengthValue"),
        slitSeparationSlider:
            document.getElementById("slitSeparationSlider"),
        slitSeparationValue:
            document.getElementById("slitSeparationValue"),
        slitWidthSlider:
            document.getElementById("slitWidthSlider"),
        slitWidthValue:
            document.getElementById("slitWidthValue"),
        screenDistanceSlider:
            document.getElementById("screenDistanceSlider"),
        screenDistanceValue:
            document.getElementById("screenDistanceValue")
    };
}

// Update all simulation data.
function updateIntensityGraph(
    engine,
    controls,
    simulationState
) {
    const physicalState =
        readPhysicalState(controls);

    applyPhysicalState(engine, physicalState);

    const waveCanvas =
        document.getElementById("waveCanvas");

    simulationState.physical = physicalState;

    simulationState.visual =
        createVisualState(
            physicalState,
            waveCanvas
        );

    // Rebuild the field after control changes.
    simulationState.interferenceField = null;

    const data =
        engine.generatePattern(-0.01, 0.01, 501);

    const positions = data.positions;
    const intensities = data.intensities;

    drawIntensityGraph(positions, intensities);

    // Copy WebAssembly data into JavaScript.
    const patternPositions = [];
    const patternIntensities = [];

    for (
        let index = 0;
        index < positions.size();
        index++
    ) {
        patternPositions.push(
            positions.get(index)
        );

        patternIntensities.push(
            intensities.get(index)
        );
    }

    simulationState.pattern = {
        positions: patternPositions,
        intensities: patternIntensities
    };

    positions.delete();
    intensities.delete();
}

// Connect controls to simulation updates.
function attachControlListeners(
    engine,
    controls,
    simulationState
) {
    const redraw = () => {
        updateIntensityGraph(
            engine,
            controls,
            simulationState
        );
    };

    controls.diffractionToggle.addEventListener(
        "change",
        redraw
    );

    const sliders = [
        controls.wavelengthSlider,
        controls.slitSeparationSlider,
        controls.slitWidthSlider,
        controls.screenDistanceSlider
    ];

    for (const slider of sliders) {
        slider.addEventListener("input", redraw);
    }
}

// Create the engine and simulation state.
function startApplication(Module) {
    const engine = new Module.DoubleSlitEngine();
    const controls = getSimulationControls();

    const simulationState = {
        physical: null,
        visual: null,
        interferenceField: null,
        pattern: {
            positions: [],
            intensities: []
        }
    };

    document.getElementById("status").textContent =
        engine.status();

    updateIntensityGraph(
        engine,
        controls,
        simulationState
    );

    attachControlListeners(
        engine,
        controls,
        simulationState
    );

    requestAnimationFrame((timestamp) => {
        animateWavefronts(timestamp, simulationState);
    });

    window.addEventListener("beforeunload", () => {
        engine.delete();
    });
}

// Load WebAssembly, then start the application.
createDoubleSlitModule().then(startApplication);

// Draw continuous ripples from both slits.
function drawWavefronts(scene, time) {
    const context = scene.context;
    const spacing = scene.wavefrontSpacingPixels;
    const visualSpeed = 70;

    const slitToScreenDistance =
        scene.screenX - scene.barrierX;

    const maximumRadius = Math.hypot(
        slitToScreenDistance,
        scene.canvas.height
    );

    const movingOffset =
        (time * visualSpeed) % spacing;

    // Restrict ripples to the experiment region.
    context.save();
    context.beginPath();
    context.rect(
        scene.barrierX,
        0,
        slitToScreenDistance,
        scene.canvas.height
    );
    context.clip();

    context.strokeStyle =
        `hsl(${scene.waveHue} 95% 72% / 1)`;

    context.lineWidth = 2.5;

    for (
        let radius = movingOffset;
        radius <= maximumRadius;
        radius += spacing
    ) {
        // Draw the upper-slit ripple.
        context.beginPath();
        context.arc(
            scene.barrierX,
            scene.upperSlitY,
            radius,
            -Math.PI / 2,
            Math.PI / 2
        );
        context.stroke();

        // Draw the lower-slit ripple.
        context.beginPath();
        context.arc(
            scene.barrierX,
            scene.lowerSlitY,
            radius,
            -Math.PI / 2,
            Math.PI / 2
        );
        context.stroke();
    }

    context.restore();
}

// Calculate interference at one canvas point.
function interferenceAtPoint(
    x,
    y,
    scene
) {
    const upperDistance = Math.hypot(
        x - scene.barrierX,
        y - scene.upperSlitY
    );

    const lowerDistance = Math.hypot(
        x - scene.barrierX,
        y - scene.lowerSlitY
    );

    const pathDifference =
        upperDistance - lowerDistance;

    const phaseDifference =
        Math.PI *
        pathDifference /
        scene.wavefrontSpacingPixels;

    return Math.pow(
        Math.cos(phaseDifference),
        2
    );
}

// Build the cached interference field.
function createInterferenceField(scene) {
    const fieldCanvas =
        document.createElement("canvas");

    fieldCanvas.width = scene.canvas.width;
    fieldCanvas.height = scene.canvas.height;

    const context =
        fieldCanvas.getContext("2d");

    const cellSize = 5;

    for (
        let y = 25;
        y < fieldCanvas.height - 25;
        y += cellSize
    ) {
        for (
            let x = scene.barrierX + 5;
            x < scene.screenX - 5;
            x += cellSize
        ) {
            const intensity =
                interferenceAtPoint(x, y, scene);

            const alpha =
                0.01 + intensity * 0.14;

            context.fillStyle =
                `hsl(${scene.waveHue} 90% 55% / ${alpha})`;

            context.fillRect(
                x,
                y,
                cellSize,
                cellSize
            );
        }
    }

    return fieldCanvas;
}

// Draw calculated intensity on the screen.
function drawScreenPattern(scene, pattern) {
    const intensities = pattern.intensities;

    if (intensities.length === 0) {
        return;
    }

    const context = scene.context;
    const screenTop = 25;
    const screenBottom = scene.canvas.height - 25;
    const screenHeight = screenBottom - screenTop;
    const screenWidth = 22;

    // Draw the screen background.
    context.fillStyle = "hsl(0 0% 0%)";

    context.fillRect(
        scene.screenX - screenWidth / 2,
        screenTop,
        screenWidth,
        screenHeight
    );

    // Draw one intensity row per pixel.
    for (
        let y = screenTop;
        y < screenBottom;
        y++
    ) {
        const verticalRatio =
            (y - screenTop) / screenHeight;

        const patternIndex = Math.round(
            (1 - verticalRatio) *
            (intensities.length - 1)
        );

        const intensity =
            intensities[patternIndex];

        const lightness =
            25 + intensity * 50;

        const alpha =
            0.08 + intensity * 0.92;

        context.fillStyle =
            `hsl(${scene.waveHue} 95% ${lightness}% / ${alpha})`;

        context.fillRect(
            scene.screenX - screenWidth / 2,
            y,
            screenWidth,
            1
        );
    }

    // Outline the viewing screen.
    context.strokeStyle = "hsl(240 5% 84%)";
    context.lineWidth = 2;

    context.strokeRect(
        scene.screenX - screenWidth / 2,
        screenTop,
        screenWidth,
        screenHeight
    );
}

let waveAnimationStart;

// Redraw geometry and moving wavefronts.
function animateWavefronts(timestamp, simulationState) {
    if (waveAnimationStart === undefined) {
        waveAnimationStart = timestamp;
    }

    const millisecondsPerSecond = 1000;
    const t =
        (timestamp - waveAnimationStart) / millisecondsPerSecond;

    const scene =
        drawWaveGeometry(simulationState.visual);

    if (simulationState.interferenceField === null) {
        simulationState.interferenceField =
            createInterferenceField(scene);
    }

    scene.context.drawImage(
        simulationState.interferenceField,
        0,
        0
    );

    drawIncomingWavefronts(scene, t);
    drawWavefronts(scene, t);
    drawScreenPattern(scene, simulationState.pattern);

    requestAnimationFrame((timestamp) => {
        animateWavefronts(timestamp, simulationState);
    });
}
