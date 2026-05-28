---
date: '2024-12-05' # Date of the post
draft: true 
title: 'Blackjack Study Game (FEH SDP)'
cover:
    image: "blackjack.png" # Assuming a cover image exists or using a placeholder name
    alt: "Blackjack Game Cover"
    caption: "Software Development Project - Fundamentals of Engineering"
tags: ["Coursework", "Software", "C++", "Game Development"]
ShowBreadCrumbs: true
---

> **tl;dr:** Partnered with a peer to develop a custom Blackjack game in C/C++ using Proteus simulator libraries, featuring custom pixel art and a gamified flashcard study system. Voted best software out of 36 projects.

## 🎯 Project Overview
* **Role:** Co-Developer (with James Butts)
* **Timeline:** Spring Semester (First Year)
* **Tech Stack / Tools:** C/C++, OSU Proteus Simulator Libraries
* **Code/Repository:** [OSU Project Site](https://u.osu.edu/fehsdp2024jet1020i2/welcome-page/)

---

## 💡 The Problem & Motivation
In our first semester of college, as part of the IBE cornerstone (first year capstone series) projects in the Honors Fundamentals of Engineering curriculum, we were tasked with creating a software application. 

When brainstorming concepts—which included ideas like a Mario-style platformer or a Temple Run endless runner—we evaluated options based on **user-experience (UX)**, **ease of play**, and **straightforward scoring**. 

We ultimately chose to build **Blackjack**. It provided a clean, simple UX reminiscent of a casino, was easy to learn, and had clear scoring rules. Beyond just a game, we added a twist: **gamifying the study process**. Players can import flashcards to study, earning chips by studying which they can then use to play blackjack. 

This game creates value by allowing people (like college students looking for quick entertainment) to experience the thrill of betting without the financial risk, potentially offering a safer alternative to real-money gambling while simultaneously encouraging studying.

## 🛠️ Design & Architecture
We utilized the OSU-developed Proteus simulator libraries and wrote the entire program in C/C++. 

The game elements we integrated included:
* **Advanced Graphics & Artistry:** We drew custom pixel art for every single card, suit, background, chip, and game layer. We also imported a custom font from a peer's GitHub library since the default Proteus libraries didn't offer enough complexity to change font size or color.
* **Random Generation:** We implemented robust shuffling algorithms to ensure the decks and hands were truly randomized for fair gameplay.
* **Multiplayer Mechanics:** We developed an automated "Dealer" logic that the user plays against.

### Game Flow
The logic behind our game is robust, handling betting, hitting, standing, doubling down, and dealer automation.
![Our Flowchart](/sdpflowchart.png)

## 📊 Results & Impact
Our hard work paid off significantly:
* **Best in Class:** We were ultimately voted as the **best software** in our class of 36 honors students.
* **Showcase Presentation:** We were selected to present our game to a panel at a showcase featuring the best projects from each section of students within the College of Engineering.

## 📸 Media Gallery
Below are some of the screens from our game, including the main menu, the dealer table, and the custom pixel art we designed.

*(If you have screenshots like menu.png or dealer.png, you can add them here as `![Menu Screen](/path-to-image.png)`)*
