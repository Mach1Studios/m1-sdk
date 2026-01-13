#!/usr/bin/env node
/**
 * Mach1 Matrices to JSON Generator
 * 
 * Parses the C++ header files containing transcode matrix definitions
 * and Mach1 spatial format definitions from Mach1EncodeCore.cpp
 * 
 * Usage:
 *   node mach1-matrices-to-json.mjs [--output <path>]
 */

import { readFileSync, writeFileSync, mkdirSync } from 'fs';
import { dirname, join, resolve } from 'path';
import { fileURLToPath } from 'url';

const __dirname = dirname(fileURLToPath(import.meta.url));

// Default paths relative to this script
const DEFAULT_MATRICES_DIR = resolve(__dirname, '../../libmach1spatial/api_transcode/matrices');
const DEFAULT_ENCODE_CORE = resolve(__dirname, '../../libmach1spatial/api_encode/src/Mach1EncodeCore.cpp');
const DEFAULT_OUTPUT_PATH = resolve(__dirname, '../public/mach1-matrices.json');

// Parse command line arguments
const args = process.argv.slice(2);
let outputPath = DEFAULT_OUTPUT_PATH;
for (let i = 0; i < args.length; i++) {
  if (args[i] === '--output' && args[i + 1]) {
    outputPath = resolve(args[i + 1]);
    i++;
  }
}

// ============================================================================
// Brace-Aware Parser Utilities
// ============================================================================

/**
 * Find matching closing brace from a given position
 */
function findMatchingBrace(content, startPos, openChar = '{', closeChar = '}') {
  let depth = 0;
  let inString = false;
  let stringChar = '';
  
  for (let i = startPos; i < content.length; i++) {
    const char = content[i];
    const prevChar = i > 0 ? content[i - 1] : '';
    
    // Handle string literals
    if ((char === '"' || char === "'") && prevChar !== '\\') {
      if (!inString) {
        inString = true;
        stringChar = char;
      } else if (char === stringChar) {
        inString = false;
      }
      continue;
    }
    
    if (inString) continue;
    
    if (char === openChar) {
      depth++;
    } else if (char === closeChar) {
      depth--;
      if (depth === 0) {
        return i;
      }
    }
  }
  
  return -1;
}

/**
 * Extract content between braces at a given position
 */
function extractBraceContent(content, startPos) {
  const openPos = content.indexOf('{', startPos);
  if (openPos === -1) return null;
  
  const closePos = findMatchingBrace(content, openPos);
  if (closePos === -1) return null;
  
  return {
    content: content.substring(openPos + 1, closePos),
    start: openPos,
    end: closePos
  };
}

/**
 * Parse a list of items separated by commas, respecting nested braces
 */
function parseCommaList(content) {
  const items = [];
  let current = '';
  let depth = 0;
  let inString = false;
  let stringChar = '';
  
  for (let i = 0; i < content.length; i++) {
    const char = content[i];
    const prevChar = i > 0 ? content[i - 1] : '';
    
    // Handle string literals
    if ((char === '"' || char === "'") && prevChar !== '\\') {
      if (!inString) {
        inString = true;
        stringChar = char;
      } else if (char === stringChar) {
        inString = false;
      }
      current += char;
      continue;
    }
    
    if (inString) {
      current += char;
      continue;
    }
    
    if (char === '{' || char === '(' || char === '[') {
      depth++;
      current += char;
    } else if (char === '}' || char === ')' || char === ']') {
      depth--;
      current += char;
    } else if (char === ',' && depth === 0) {
      const trimmed = current.trim();
      if (trimmed) items.push(trimmed);
      current = '';
    } else {
      current += char;
    }
  }
  
  const trimmed = current.trim();
  if (trimmed) items.push(trimmed);
  
  return items;
}

// ============================================================================
// Mach1 Spatial Format Definitions Parser
// ============================================================================

/**
 * Evaluate a simple math expression like "1 / 0.707" or "-1.310"
 */
function evaluateMathExpression(expr) {
  // Clean up the expression
  expr = expr.trim().replace(/f$/i, '');
  
  // Handle division like "1 / 0.707"
  if (expr.includes('/')) {
    const parts = expr.split('/').map(p => p.trim());
    if (parts.length === 2) {
      const num = parseFloat(parts[0]);
      const denom = parseFloat(parts[1]);
      if (!isNaN(num) && !isNaN(denom) && denom !== 0) {
        return num / denom;
      }
    }
  }
  
  // Handle multiplication
  if (expr.includes('*')) {
    const parts = expr.split('*').map(p => p.trim());
    if (parts.length === 2) {
      const a = parseFloat(parts[0]);
      const b = parseFloat(parts[1]);
      if (!isNaN(a) && !isNaN(b)) {
        return a * b;
      }
    }
  }
  
  // Simple number
  return parseFloat(expr);
}

/**
 * Parse Mach1 spatial format definitions from Mach1EncodeCore.cpp
 */
function parseMach1SpatialDefinitions(content) {
  const spatialFormats = {};
  
  // Parse point definitions like: m1Spatial_8_Def = { {x, y, z}, ... }
  const pointDefRegex = /m1Spatial_(\d+)_Def\s*=\s*\{/g;
  let match;
  
  while ((match = pointDefRegex.exec(content)) !== null) {
    const channels = parseInt(match[1]);
    const braceContent = extractBraceContent(content, match.index);
    if (!braceContent) continue;
    
    const points = [];
    const pointMatches = braceContent.content.matchAll(/\{([^}]+)\}/g);
    
    for (const pm of pointMatches) {
      // Split by comma but handle expressions with spaces
      const coordStrs = pm[1].split(',').map(s => s.trim());
      if (coordStrs.length >= 3) {
        const x = evaluateMathExpression(coordStrs[0]);
        const y = evaluateMathExpression(coordStrs[1]);
        const z = evaluateMathExpression(coordStrs[2]);
        if (!isNaN(x) && !isNaN(y) && !isNaN(z)) {
          points.push({ x, y, z });
        }
      }
    }
    
    if (points.length > 0) {
      const name = `M1Spatial-${channels}`;
      if (!spatialFormats[name]) {
        spatialFormats[name] = { name, numChannels: channels, points: [], lines: [], planes: [] };
      }
      spatialFormats[name].points = points;
    }
  }
  
  // Parse line definitions like: m1Spatial_8_Lines_Def = { {0, 1}, ... }
  const lineDefRegex = /m1Spatial_(\d+)_Lines_Def\s*=\s*\{/g;
  
  while ((match = lineDefRegex.exec(content)) !== null) {
    const channels = parseInt(match[1]);
    const braceContent = extractBraceContent(content, match.index);
    if (!braceContent) continue;
    
    const lines = [];
    const lineMatches = braceContent.content.matchAll(/\{(\d+)\s*,\s*(\d+)\}/g);
    
    for (const lm of lineMatches) {
      lines.push([parseInt(lm[1]), parseInt(lm[2])]);
    }
    
    const name = `M1Spatial-${channels}`;
    if (spatialFormats[name]) {
      spatialFormats[name].lines = lines;
    }
  }
  
  // Parse plane definitions like: m1Spatial_8_Plane_Def = { {0, 1, 2}, ... }
  const planeDefRegex = /m1Spatial_(\d+)_Plane_Def\s*=\s*\{/g;
  
  while ((match = planeDefRegex.exec(content)) !== null) {
    const channels = parseInt(match[1]);
    const braceContent = extractBraceContent(content, match.index);
    if (!braceContent) continue;
    
    const planes = [];
    const planeMatches = braceContent.content.matchAll(/\{(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\}/g);
    
    for (const pm of planeMatches) {
      planes.push([parseInt(pm[1]), parseInt(pm[2]), parseInt(pm[3])]);
    }
    
    const name = `M1Spatial-${channels}`;
    if (spatialFormats[name]) {
      spatialFormats[name].planes = planes;
    }
  }
  
  return spatialFormats;
}

// ============================================================================
// Matrix Parsing
// ============================================================================

/**
 * Parse a Coeffs channel: Mach1TranscodeChannel::Coeffs({ ... })
 */
function parseCoeffsChannel(str) {
  const match = str.match(/Coeffs\s*\(\s*\{([^}]*)\}/);
  if (!match) return null;
  
  const coeffs = match[1].split(',')
    .map(s => s.trim())
    .filter(s => s.length > 0)
    .map(s => {
      // Handle expressions like 0.5f, -1.0, sqrt(2)/2, etc.
      s = s.replace(/f$/i, '');
      if (s.includes('sqrt')) {
        // Approximate sqrt expressions
        const sqrtMatch = s.match(/sqrt\((\d+(?:\.\d+)?)\)\s*\/\s*(\d+)/);
        if (sqrtMatch) {
          return Math.sqrt(parseFloat(sqrtMatch[1])) / parseFloat(sqrtMatch[2]);
        }
      }
      return parseFloat(s);
    });
  
  return { kind: 'Coeffs', data: coeffs };
}

/**
 * Parse a Panner channel: Mach1TranscodeChannel::Panner(azimuth, elevation, diverge[, gain])
 */
function parsePannerChannel(str) {
  // Match Panner with 3 or 4 arguments
  const match = str.match(/Panner\s*\(\s*(-?[\d.]+)\s*,\s*(-?[\d.]+)\s*,\s*(-?[\d.]+)(?:\s*,\s*(-?[\d.]+))?\s*\)/);
  if (!match) return null;
  
  const result = {
    kind: 'Panner',
    azimuth: parseFloat(match[1]),
    elevation: parseFloat(match[2]),
    diverge: parseFloat(match[3])
  };
  
  if (match[4] !== undefined) {
    result.gain = { value: parseFloat(match[4]), isDecibel: true };
  }
  
  return result;
}

/**
 * Parse a single matrix entry: {"formatFrom", "formatTo", { channels... }}
 */
function parseMatrixEntry(content) {
  // Extract the three main parts
  const firstBrace = content.indexOf('{');
  if (firstBrace === -1) return null;
  
  // Find format strings
  const stringMatches = [...content.matchAll(/"([^"]+)"/g)];
  if (stringMatches.length < 2) return null;
  
  const formatFrom = stringMatches[0][1];
  const formatTo = stringMatches[1][1];
  
  // Find channels array - it's the third opening brace (after the two format strings)
  // First brace is the matrix entry itself, second brace is inside stringMatches[1], 
  // third brace is the channels array
  const secondStringEnd = stringMatches[1].index + stringMatches[1][0].length;
  const channelsStart = content.indexOf('{', secondStringEnd);
  if (channelsStart === -1) return null;
  
  const channelsEnd = findMatchingBrace(content, channelsStart);
  if (channelsEnd === -1) return null;
  
  const channelsContent = content.substring(channelsStart + 1, channelsEnd);
  
  // Parse each channel
  const channels = [];
  const channelItems = parseCommaList(channelsContent);
  
  for (const item of channelItems) {
    if (item.includes('Coeffs')) {
      const parsed = parseCoeffsChannel(item);
      if (parsed) channels.push(parsed);
    } else if (item.includes('Panner')) {
      const parsed = parsePannerChannel(item);
      if (parsed) channels.push(parsed);
    }
  }
  
  return { formatFrom, formatTo, channels };
}

/**
 * Parse all matrices from a header file
 */
function parseMatricesFromFile(content, group) {
  const matrices = [];
  
  // Find getData() function and extract its return statement
  const getDataMatch = content.match(/getData\s*\(\s*\)\s*\{[\s\S]*?return\s*\{/);
  if (!getDataMatch) {
    console.warn(`Warning: Could not find getData() in ${group}Formats.h`);
    return matrices;
  }
  
  const returnStart = getDataMatch.index + getDataMatch[0].length - 1;
  const returnEnd = findMatchingBrace(content, returnStart);
  if (returnEnd === -1) {
    console.warn(`Warning: Could not find matching brace for getData() return in ${group}Formats.h`);
    return matrices;
  }
  
  const returnContent = content.substring(returnStart + 1, returnEnd);
  
  // Split into individual matrix entries
  // Each entry is { "from", "to", { channels } }
  let depth = 0;
  let currentEntry = '';
  let entries = [];
  
  for (let i = 0; i < returnContent.length; i++) {
    const char = returnContent[i];
    
    if (char === '{') {
      if (depth === 0) {
        currentEntry = '{';
      } else {
        currentEntry += char;
      }
      depth++;
    } else if (char === '}') {
      depth--;
      currentEntry += char;
      if (depth === 0) {
        entries.push(currentEntry);
        currentEntry = '';
      }
    } else if (depth > 0) {
      currentEntry += char;
    }
  }
  
  // Parse each entry
  for (const entry of entries) {
    const parsed = parseMatrixEntry(entry);
    if (parsed) {
      matrices.push({ group, ...parsed });
    }
  }
  
  return matrices;
}

/**
 * Parse format list from constants file
 */
function parseFormatList(content) {
  const formats = [];
  
  // Look for the formats array/enum
  const formatMatches = content.matchAll(/"([^"]+)"/g);
  const seen = new Set();
  
  for (const match of formatMatches) {
    const name = match[1];
    // Filter out non-format strings
    if (name && !seen.has(name) && !name.includes(' ') && name.length < 50) {
      seen.add(name);
      formats.push({ name, numChannels: 0, channelTypes: [] });
    }
  }
  
  return formats;
}

// ============================================================================
// Standard Surround Format Positions
// ============================================================================

/**
 * Standard speaker positions for common surround formats
 * Coordinate system: X = Left(-) to Right(+), Y = Front(+) to Back(-), Z = Top(+) to Bottom(-)
 * Positions use normalized unit sphere/cube coordinates
 */
const STANDARD_FORMAT_POSITIONS = {
  // ---- Stereo / LCR ----
  '1.0': [
    { x: 0, y: 1, z: 0, label: 'C' }  // Center/Mono
  ],
  '2.0_M': [
    { x: -0.5, y: 1, z: 0, label: 'L' },
    { x: 0.5, y: 1, z: 0, label: 'R' }
  ],
  '2.0_C': [
    { x: -0.5, y: 1, z: 0, label: 'L' },
    { x: 0.5, y: 1, z: 0, label: 'R' }
  ],
  '3.0_LCR': [
    { x: -0.5, y: 1, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 1, z: 0, label: 'R' }
  ],
  
  // ---- 5.x Surround ----
  '5.0_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },      // 30 deg left
    { x: 0, y: 1, z: 0, label: 'C' },              // Center
    { x: 0.5, y: 0.866, z: 0, label: 'R' },       // 30 deg right
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },   // 120 deg left (rear)
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' }     // 120 deg right (rear)
  ],
  '5.0_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' }
  ],
  '5.1_M': [  // L C R Ls Rs LFE
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' }  // LFE below front
  ],
  '5.1_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' }
  ],
  '5.1_M_SMPTE': [  // L R C LFE Ls Rs
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' }
  ],
  
  // ---- 5.x.2 Height (Atmos/Auro) ----
  '5.0.2_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },  // Top front left
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' }   // Top front right
  ],
  '5.1.2_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' }
  ],
  
  // ---- 5.x.4 Height ----
  '5.0.4_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' },
    { x: -0.866, y: -0.5, z: 0.7, label: 'Ltr' },  // Top rear left
    { x: 0.866, y: -0.5, z: 0.7, label: 'Rtr' }   // Top rear right
  ],
  '5.1.4_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -0.866, y: -0.5, z: 0, label: 'Ls' },
    { x: 0.866, y: -0.5, z: 0, label: 'Rs' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' },
    { x: -0.866, y: -0.5, z: 0.7, label: 'Ltr' },
    { x: 0.866, y: -0.5, z: 0.7, label: 'Rtr' }
  ],
  
  // ---- 7.x Surround ----
  '7.0_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },    // Side left (90 deg)
    { x: 1, y: 0, z: 0, label: 'Rss' },     // Side right (90 deg)
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },  // Rear left (135 deg)
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' }   // Rear right (135 deg)
  ],
  '7.0_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' }
  ],
  '7.1_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' }
  ],
  '7.1_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' }
  ],
  
  // ---- 7.x.2 Height ----
  '7.0.2_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' }
  ],
  '7.1.2_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' }
  ],
  '7.0.2_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' }
  ],
  '7.1.2_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' }
  ],
  
  // ---- 7.x.4 Height ----
  '7.0.4_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' },
    { x: -0.707, y: -0.707, z: 0.7, label: 'Ltr' },
    { x: 0.707, y: -0.707, z: 0.7, label: 'Rtr' }
  ],
  '7.1.4_M': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' },
    { x: -0.707, y: -0.707, z: 0.7, label: 'Ltr' },
    { x: 0.707, y: -0.707, z: 0.7, label: 'Rtr' }
  ],
  '7.0.4_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' },
    { x: -0.707, y: -0.707, z: 0.7, label: 'Ltr' },
    { x: 0.707, y: -0.707, z: 0.7, label: 'Rtr' }
  ],
  '7.1.4_C': [
    { x: -0.5, y: 0.866, z: 0, label: 'L' },
    { x: 0, y: 1, z: 0, label: 'C' },
    { x: 0.5, y: 0.866, z: 0, label: 'R' },
    { x: -1, y: 0, z: 0, label: 'Lss' },
    { x: 1, y: 0, z: 0, label: 'Rss' },
    { x: -0.707, y: -0.707, z: 0, label: 'Lsr' },
    { x: 0.707, y: -0.707, z: 0, label: 'Rsr' },
    { x: 0, y: 0.5, z: -0.5, label: 'LFE' },
    { x: -0.5, y: 0.866, z: 0.7, label: 'Ltf' },
    { x: 0.5, y: 0.866, z: 0.7, label: 'Rtf' },
    { x: -0.707, y: -0.707, z: 0.7, label: 'Ltr' },
    { x: 0.707, y: -0.707, z: 0.7, label: 'Rtr' }
  ],
  
  // ---- Quad / LCRs ----
  'Quad': [
    { x: -0.707, y: 0.707, z: 0, label: 'FL' },
    { x: 0.707, y: 0.707, z: 0, label: 'FR' },
    { x: -0.707, y: -0.707, z: 0, label: 'RL' },
    { x: 0.707, y: -0.707, z: 0, label: 'RR' }
  ],
  'M1Horizon': [
    { x: -0.707, y: 0.707, z: 0, label: 'FL' },
    { x: 0.707, y: 0.707, z: 0, label: 'FR' },
    { x: -0.707, y: -0.707, z: 0, label: 'RL' },
    { x: 0.707, y: -0.707, z: 0, label: 'RR' }
  ]
};

// ============================================================================
// Main
// ============================================================================

async function main() {
  console.log('Mach1 Matrices to JSON Generator');
  console.log('=================================\n');
  
  const matrixFiles = [
    { file: 'VectorFormats.h', group: 'Vector' },
    { file: 'SurroundFormats.h', group: 'Surround' },
    { file: 'MicArrayFormats.h', group: 'MicArray' },
    { file: 'AmbisonicFormats.h', group: 'Ambisonic' }
  ];
  
  const allMatrices = [];
  const formatsReferenced = { byGroup: {}, all: new Set() };
  
  // Parse matrix files
  for (const { file, group } of matrixFiles) {
    const filePath = join(DEFAULT_MATRICES_DIR, file);
    console.log(`Parsing ${file}...`);
    
    try {
      const content = readFileSync(filePath, 'utf-8');
      const matrices = parseMatricesFromFile(content, group);
      
      console.log(`  Found ${matrices.length} matrices`);
      allMatrices.push(...matrices);
      
      // Track referenced formats
      formatsReferenced.byGroup[group] = new Set();
      for (const m of matrices) {
        formatsReferenced.byGroup[group].add(m.formatFrom);
        formatsReferenced.byGroup[group].add(m.formatTo);
        formatsReferenced.all.add(m.formatFrom);
        formatsReferenced.all.add(m.formatTo);
      }
    } catch (err) {
      console.error(`  Error reading ${filePath}: ${err.message}`);
    }
  }
  
  // Parse Mach1 spatial definitions from encode core
  console.log(`\nParsing Mach1EncodeCore.cpp for spatial definitions...`);
  let mach1SpatialFormats = {};
  
  try {
    const encodeContent = readFileSync(DEFAULT_ENCODE_CORE, 'utf-8');
    mach1SpatialFormats = parseMach1SpatialDefinitions(encodeContent);
    console.log(`  Found ${Object.keys(mach1SpatialFormats).length} Mach1 spatial format definitions`);
    
    for (const [name, format] of Object.entries(mach1SpatialFormats)) {
      console.log(`    ${name}: ${format.points.length} points, ${format.lines.length} lines, ${format.planes.length} planes`);
    }
  } catch (err) {
    console.error(`  Error reading Mach1EncodeCore.cpp: ${err.message}`);
  }
  
  // Parse format list
  console.log(`\nParsing Mach1TranscodeConstants.h...`);
  let formats = [];
  
  try {
    const constantsPath = join(DEFAULT_MATRICES_DIR, 'Mach1TranscodeConstants.h');
    const constantsContent = readFileSync(constantsPath, 'utf-8');
    formats = parseFormatList(constantsContent);
    console.log(`  Found ${formats.length} format names`);
  } catch (err) {
    console.error(`  Error: ${err.message}`);
  }
  
  // Build format positions from:
  // 1. Standard format positions (known speaker layouts)
  // 2. Panner data from matrices (if available)
  const formatPositions = {};
  
  // First, add all known standard format positions
  for (const [formatName, positions] of Object.entries(STANDARD_FORMAT_POSITIONS)) {
    formatPositions[formatName] = positions;
  }
  
  // Then, extract any additional positions from matrices with Panner data
  for (const matrix of allMatrices) {
    const pannerChannels = matrix.channels.filter(ch => ch.kind === 'Panner');
    
    if (pannerChannels.length > 0 && !formatPositions[matrix.formatFrom]) {
      // Store positions for the source format if not already defined
      formatPositions[matrix.formatFrom] = pannerChannels.map(ch => ({
        azimuth: ch.azimuth,
        elevation: ch.elevation,
        diverge: ch.diverge,
        gain: ch.gain
      }));
    }
  }
  
  console.log(`  Standard format positions: ${Object.keys(STANDARD_FORMAT_POSITIONS).length}`);
  console.log(`  Total format positions: ${Object.keys(formatPositions).length}`);
  
  // Convert sets to arrays
  const outputFormatsReferenced = {
    byGroup: {},
    all: [...formatsReferenced.all].sort()
  };
  
  for (const [group, set] of Object.entries(formatsReferenced.byGroup)) {
    outputFormatsReferenced.byGroup[group] = [...set].sort();
  }
  
  // Build output
  const output = {
    generatedAt: new Date().toISOString(),
    mach1SpatialFormats: mach1SpatialFormats,
    formatPositions: formatPositions,
    formats: formats,
    matrices: allMatrices,
    formatsReferenced: outputFormatsReferenced
  };
  
  // Ensure output directory exists
  const outputDir = dirname(outputPath);
  mkdirSync(outputDir, { recursive: true });
  
  // Write output
  writeFileSync(outputPath, JSON.stringify(output, null, 2));
  console.log(`\nOutput written to: ${outputPath}`);
  console.log(`Total matrices: ${allMatrices.length}`);
  console.log(`Mach1 spatial formats: ${Object.keys(mach1SpatialFormats).length}`);
  console.log(`Format positions extracted: ${Object.keys(formatPositions).length}`);
}

main().catch(err => {
  console.error('Fatal error:', err);
  process.exit(1);
});
