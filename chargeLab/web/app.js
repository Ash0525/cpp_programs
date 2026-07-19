let simulation = null;
let canvas = null;
let ctx = null;

let lastTime = 0;
let physicsCrashed = false;

let isDraggingSelected = false;
let mouseX = 0;
let mouseY = 0;

const MAX_FRAME_DT = 1.0 / 20.0;
const MAX_SUBSTEP_DT = 1.0 / 240.0;

function isFiniteParticleValue(value) {
    return Number.isFinite(value);
}

function stepSimulationSafely(frameDt) {
    let remaining = Math.max(0, Math.min(frameDt, MAX_FRAME_DT));

    while (remaining > 0) {
        const stepDt = Math.min(remaining, MAX_SUBSTEP_DT);
        simulation.update(stepDt);
        remaining -= stepDt;
    }
}

function drawParticles() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    drawForceVectors();

    const particleCount = simulation.getParticleCount();

    for (let i = 0; i < particleCount; i++) {
        const x = simulation.getParticleX(i);
        const y = simulation.getParticleY(i);
        const radius = simulation.getParticleRadius(i);
        const charge = simulation.getParticleCharge(i);

        if (!isFiniteParticleValue(x) ||
            !isFiniteParticleValue(y) ||
            !isFiniteParticleValue(radius) ||
            radius <= 0) {
            console.warn("Skipping particle with invalid numeric state", {
                index: i,
                x,
                y,
                radius,
                charge
            });
            continue;
        }

        if (charge > 0) {
            ctx.fillStyle = "red";
        }
        else if (charge < 0) {
            ctx.fillStyle = "blue";
        }
        else {
            ctx.fillStyle = "white";
        }

        ctx.beginPath();
        ctx.arc(x, y, radius, 0, 2 * Math.PI);
        ctx.fill();

        if (simulation.hasSelected() && i === simulation.getSelectedIndex()) {
            ctx.strokeStyle = "yellow";
            ctx.lineWidth = 4;
            ctx.beginPath();
            ctx.arc(x, y, radius + 3, 0, 2 * Math.PI);
            ctx.stroke();
        }
    }
}

function animationLoop(currentTime) {
    const dt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    if (simulation !== null && !physicsCrashed) {
        try {
            if (Number.isFinite(dt) && dt >= 0) {
                if (isDraggingSelected && simulation.hasSelected()) {
                    simulation.moveSelected(mouseX, mouseY);
                    simulation.setSelectedVelocity(0.0, 0.0);
                }

                stepSimulationSafely(dt);

                if (isDraggingSelected && simulation.hasSelected()) {
                    simulation.moveSelected(mouseX, mouseY);
                    simulation.setSelectedVelocity(0.0, 0.0);
                }
            }

            drawParticles();
        } catch (error) {
            physicsCrashed = true;
            console.error("Simulation failed somehow ya noob", error);
        }
    }
    requestAnimationFrame(animationLoop);
}

// Mouse helper functions

function getMousePosition(event) {

    // Identify where the canvas is and how big it is
    const rect = canvas.getBoundingClientRect();

    // Visual resize for CSS
    const scaleX = canvas.width / rect.width;
    const scaleY = canvas.height / rect.height;

    // Small state pieces that remember what the mouse is doing
    mouseX = (event.clientX - rect.left) * scaleX;
    mouseY = (event.clientY - rect.top) * scaleY;
}

function setupMouseControls() {
    canvas.addEventListener("mousedown", (event) => {
        getMousePosition(event);

        // Boolean in selected if the mouse is at particle
        const selected = simulation.selectParticleAt(mouseX, mouseY);

        // If selected is true, then dragging is true
        if (selected) {
            isDraggingSelected = true;
        }
        else {
            simulation.clearSelected();
            isDraggingSelected = false;
        }
    });

    canvas.addEventListener("mousemove", (event) => {
        getMousePosition(event);

        if (isDraggingSelected && simulation.hasSelected()) {
            simulation.moveSelected(mouseX, mouseY);
            simulation.setSelectedVelocity(0.0, 0.0);
        }
    });

    // When the user releases the mouse button
    canvas.addEventListener("mouseup", () => {
        isDraggingSelected = false;
    });

    // When the mouse leaves the canvas area
    canvas.addEventListener("mouseleave", () => {
        isDraggingSelected = false;
    });
}

// Vector Drawing
function drawArrow(startX, startY, endX, endY, color) {
    const dx = endX - startX;
    const dy = endY - startY;
    const arrowLength = Math.hypot(dx, dy);

    if (!Number.isFinite(arrowLength) || arrowLength <= 0) {
        return;
    }

    const headLength = Math.max(6, Math.min(18, arrowLength * 0.25));
    const angle = Math.atan2(endY - startY, endX - startX);

    ctx.strokeStyle = color;
    ctx.fillStyle = color;
    ctx.lineWidth = 3;

    ctx.beginPath();
    ctx.moveTo(startX, startY);
    ctx.lineTo(endX, endY);
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(endX, endY);
    
    // Draw the arrow heads
    ctx.lineTo(
        endX - headLength * Math.cos(angle - Math.PI / 6),
        endY - headLength * Math.sin(angle - Math.PI / 6)
    );

    ctx.lineTo(
        endX - headLength * Math.cos(angle + Math.PI / 6),
        endY - headLength * Math.sin(angle + Math.PI / 6)
    );

    ctx.closePath();
    ctx.fill();
}

function getForceArrowLength(forceMagnitude) {
    if (!Number.isFinite(forceMagnitude) || forceMagnitude <= 0) {
        return 0;
    }

    // Minimum arrow length
    const minArrowLength = 8;
    const maxArrowLength = 120;
    const scaledLength = Math.sqrt(forceMagnitude) * 6;

    return Math.max(minArrowLength, Math.min(maxArrowLength, scaledLength));
}

// Draw force vectors
// Takes the drawArrow function and applies the C++ physics to it

function drawForceVectors() {
    if (!simulation.hasSelected()) {
        return;
    }

    // Get the index of the selected particle
    const selectedIndex = simulation.getSelectedIndex();

    const startX = simulation.getParticleX(selectedIndex);
    const startY = simulation.getParticleY(selectedIndex);

    // Get the particle count
    const particleCount = simulation.getParticleCount();

    let forceData = [];

    for (let i = 0; i < particleCount; i++) {
        if (i === selectedIndex) {
            continue;
        }

        const force = simulation.getForceBetweenParticles(selectedIndex, i);

        const fx = force.getX();
        const fy = force.getY();
        const magnitude = force.magnitude();

        force.delete();

        forceData.push({ fx, fy, magnitude });
    }

    for (const force of forceData) {
        if (!Number.isFinite(force.magnitude) || force.magnitude <= 0) {
            continue;
        }

        // Normalize force vector
        const unitX = force.fx / force.magnitude;
        const unitY = force.fy / force.magnitude;

        const arrowLength = getForceArrowLength(force.magnitude);

        const endX = startX + unitX * arrowLength;
        const endY = startY + unitY * arrowLength;

        drawArrow(startX, startY, endX, endY, "yellow");
    }
}

// Add functionality to the buttons. Pause and clear buttons are in here together
function setupButtons() {

    // Simulation Control
    const pauseButton = document.getElementById("pauseButton");
    const clearButton = document.getElementById("clearButton");

    // Interaction Control
    const selectModeButton = document.getElementById("selectedModeButton");
    const positiveModeButton = document.getElementById("positiveModeButton");
    const negativeModeButton = document.getElementById("negativeModeButton");

    // Pause button functionality
    pauseButton.addEventListener("click", () => {
        // pause method from simulation
        simulation.togglePaused();

        // if isPaused is true or false
        if (simulation.isPaused()) {
            pauseButton.textContent = "Play";
        }
        else {
            pauseButton.textContent = "Pause";
        }
    });

    // Clear button functionality
    clearButton.addEventListener("click", () => {
        simulation.clearParticles();
        simulation.clearSelected();
    });

    // Select Mode button functionality
    selectModeButton.addEventListener("click", () => {
        currentMode = "select";
        console.log("Mode: select");
    });

    // Positive Mode button functionality
    positiveModeButton.addEventListener("click", () => {
        currentMode = "positive";
        console.log("Mode: positive");
    });

    // Negative Mode button functionality
    negativeModeButton.addEventListener("click", () => {
        currentMode = "negative";
        console.log("Mode: negative");
    });
}

var Module = {
    onRuntimeInitialized: function () {
        console.log("ChargeLab WASM loaded!");

        canvas = document.getElementById("simCanvas");
        ctx = canvas.getContext("2d");

        simulation = new Module.Simulation(900, 700);

        // Mouse controls
        setupMouseControls();

        // Clear and pause controls
        setupButtons();

        simulation.addParticleAtFull(300, 350, 1.0, 10.0, 18.0);
        simulation.addParticleAtFull(600, 350, -1.0, 10.0, 18.0);

        simulation.setME0();

        console.log("Particle count:", simulation.getParticleCount());

        lastTime = performance.now();
        requestAnimationFrame(animationLoop);
    }
};