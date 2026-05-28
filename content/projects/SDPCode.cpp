/*references: 
    github.com/Feltlin/FEH_Proteus/blob/main/Text.h
    https://www.geeksforgeeks.org/shuffle-a-deck-of-cards-3/
    https://u.osu.edu/fehproteus/
*/ 
#include "FEHLCD.h"
#include "FEHRandom.h"
#include "FEHUtility.h"
#include “FEHImages.h”
#include “text.h”
#define max_decks 4
#define decksize 52 //number of cards in deck
#define middlex 160
#define middley 120
//coords x(0,319)  y(0,239)
//initializing variables
Text text;
int currentCard = 0;
void menu_button(int button_color, int recty, char name[], int textx, int texty);
void player_buttons(int button_color, int recty, char name[]);
void betting_buttons(int button_color, int recty, char name[]);
float center(float centerx, float length, float spacing, float num);
int numDecks = 1; // Control
int totalCards = numDecks * decksize;
int hit_num;
int dealer_hit_num;
int index = 0;
bool blackJack;
bool round_state = true;
bool nobust = true;
bool playing_state = true;
bool betting_state = true;
bool touched = true;
bool doubleDown = false;
bool haschips = true;
char statement[20];
char walletchar[20];
char dealerhandtotal[20];
int wallet = 1000;
int totalbet = 0;
int profit = 0;
int bet = 0;
int redChipAmount = 0;
int greenChipAmount = 0;
int blueChipAmount = 0;
FEHImage fiftyChip;
FEHImage twentyChip;
FEHImage tenChip;
void updateChips(); //update wallets
int exitButton(int state, float x_pos, float y_pos); //creates exit button for non menu states
void dealerHit(); //dealer hits if cards <17, otherwise stands(or busted)
void dealertotal(); //calculates total of dealer’s card values
class cards
{
    public:
    cards(char [] = “na”,char [] = “na”);
    char rank[3];
    char suit[10];
    void printCard(int, int, int);
};
cards Deck[208]; //problem before was that you can’t have runtime variables for size of array. to solve this, we use 4 decks
class Dealer
{
    public:
        Dealer();
        void flipCard();
        int handCount;
        int flippedCard;
        int aceInHand;
};
class Player
{
    public:
        Player();
        int handCount;
        int wins;
        int losses;
        int ties;
        int aceInHand;
        void hit();
};
// Create dealer and player objects
            Dealer dealer;
            Player player;
void initializeDecks();
void shuffleDeck(cards d[], int totalC);
void dealCards(); //deals cards out
void checkAce(); //checks if dealer or player has ace; useful to converrt ace value to 1 if value is appropriate
void checkDealerWin(); //checks if dealer has blackjack on deal; takes your money so you don’t have to bet
void checkBlackjack(); //checks if you have blackjack, then stands
void updateBet(); //updates drawn bets on the screen
int main()
{
    int x_pos, y_pos;
    //colors used(RGB int) http://www.shodor.org/~efarrow/trunk/html/rgbint.html
    int bg_col = 3289650, start_col = 14474460, menu_button_col = 5921370, hover_col = 1315860;
    int state = 0; //game state, 0 = menu, 1= game, 2= stats, 3= credits
    bool cont1= true;
    FEHImage Logo;                 //https://www.pixilart.com/draw#
    Logo.Open(“logo5.png”);
    //while game is active(always)
    while(1){
    //Menu State w/ start, instructions, credits, stats buttons
    while (state == 0){
        LCD.SetBackgroundColor(bg_col);
        LCD.Clear();
        cont1 = true;
        //opening blackjack logo
        Logo.Draw(12, 20);
        round_state = true;
        //detect hover
        while(!LCD.Touch(&x_pos,&y_pos)) {
        //change color
        if(x_pos >= 60 && x_pos <= 260 && y_pos >= 75 && y_pos <= 105 ){
            menu_button(hover_col, 75, “Start”, center(160, 12, 0, 5), 82);
        }else if(x_pos >= 60 && x_pos <= 260 && y_pos >= 120 && y_pos <= 150 ){
            menu_button(hover_col, 120, “Instructions”, center(160, 12, 0, 12), 127);
        }else if(x_pos >= 60 && x_pos <= 260 && y_pos >= 165 && y_pos <= 195 ){
            menu_button(hover_col, 165, “Stats”, 130, 172);
        }else if(x_pos >= 60 && x_pos <= 260 && y_pos >= 210 && y_pos <= 239 ){
            menu_button(hover_col, 209, “Credits”, 118, 217);
        }else{
            //drawing menu squares and text
            menu_button(start_col, 75, “Start”, 130, 82); //start button
            menu_button(menu_button_col, 120, “Instructions”, 88, 127); //instructions button
            menu_button(menu_button_col, 165, “Stats”, 130, 172); //stats button
            menu_button(menu_button_col, 209, “Credits”, 118, 217); //credits button
        }
        LCD.Update();
        };
        //detect press(drag)
        while(LCD.Touch(&x_pos,&y_pos)) {
        LCD.Update();
        LCD.SetBackgroundColor(WHITE);};
        //if start hit, start game
        if(x_pos >= 60 && x_pos <= 260 && y_pos >= 75 && y_pos <= 105 ){
            state = 1;
        }
        //if instructions hit, go to instructions
        if(x_pos >= 60 && x_pos <= 260 && y_pos >= 120 && y_pos <= 150 ){
            state = 2;
        }
        //if stats hit, go to stats
        if(x_pos >= 60 && x_pos <= 260 && y_pos >= 165 && y_pos <= 195 ){
            state = 3;
        }
        //if credits hit, go to credits
        if(x_pos >= 60 && x_pos <= 260 && y_pos >= 210 && y_pos <= 239 ){
            state = 4;
        }
    }
    //game
    while(state == 1){
        while(round_state)
        {
            LCD.Clear();
            FEHImage background;
            FEHImage dealerImage;
            background.Open(“background.png”);
            background.Draw(0,0);
            dealerImage.Open(“toney.png”);
            dealerImage.Draw(135,25);
            blueChipAmount = 0;
            greenChipAmount = 0;
            redChipAmount = 0;
            betting_state = true;
            bet = 0;
            updateChips();
            if(wallet < 10){ //game over screen
                betting_state = false;
                char endtext[25];
                sprintf(endtext, “Out of Chips, Game Over!”);
                LCD.WriteAt(endtext, center(middlex, 12, 0, strlen(endtext)), center(middley, 17, 0, 2));
                LCD.Update();
                Sleep(2000);
                round_state = false;
                betting_state = false;
                playing_state = false;
                haschips = false;
                state == 0;
            } else{
                while(betting_state) //stuff when you’re betting
                {
                    playing_state = false;
                    while(!LCD.Touch(&x_pos,&y_pos)) {
                        //change color
                            if(x_pos >= 30 && x_pos <= 80 && y_pos >= 100 && y_pos <= 120 ){
                                betting_buttons(hover_col, 100, “+ 10”);
                            }else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 130 && y_pos <= 150 ){
                                betting_buttons(hover_col, 130, “+ 20”);
                            }else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 160 && y_pos <= 180 ){
                                betting_buttons(hover_col, 160, “+ 50”);
                            }else{
                                //drawing menu squares and text
                                betting_buttons(menu_button_col, 100, “+ 10”); //+10 button
                                betting_buttons(menu_button_col, 130, “+ 20”); //+20 button
                                betting_buttons(menu_button_col, 160, “+ 50”); //+50 button
                                //Done button
                                sprintf(statement, “Done”);
                                LCD.SetFontColor(BLACK);
                                LCD.FillRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                                LCD.SetFontColor(LIGHTBLUE);
                                LCD.DrawRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                                text.display(statement, LIGHTBLUE, center(160, 5, 1, strlen(statement)), center(200, 7, 0, 1)+30);
                                LCD.Update();
                            }   //center(80, 12, 0, 6)
                            LCD.Update();
                        }
                        while(LCD.Touch(&x_pos,&y_pos)){
                            //grabs x and y pos
                            touched = true;
                        }
                        while(!LCD.Touch(&x_pos,&y_pos) && touched){
                            if(x_pos >= 30 && x_pos <= 80 && y_pos >= 100 && y_pos <= 120 && wallet >= 10){ // if + 10 pressed
                                bet += 10;
                                redChipAmount++;
                                wallet -= 10;
                                updateBet();
                                updateChips();
                                touched = false;
                            }
                            else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 130 && y_pos <= 150 && wallet >= 20 ){ //if + 20 pressed
                                bet +=20;
                                greenChipAmount++;
                                wallet -= 20;
                                updateBet();
                                updateChips();
                                touched = false;
                            }
                            else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 160 && y_pos <= 180 && wallet >= 50){ //if + 50 is pressed
                                bet +=50;
                                blueChipAmount++;
                                wallet -= 50;
                                updateBet();
                                updateChips();
                                touched = false;
                            }
                            else if (x_pos >= center(160, 60, 0, 1) && x_pos <= center(160, 60, 0, 1)+60 && y_pos >= center(200, 15, 0, 1)+30 && y_pos <= center(200, 15, 0, 1)+45 && bet >0){ //if done is pressed
                                betting_state = false;
                                playing_state = true;
                                updateChips();
                                updateBet();
                                touched = false;
                            }
                        }
                }
                    updateBet();
                    // initialize + shuffle deck and then deal cards
                    initializeDecks();
                    shuffleDeck(Deck, totalCards);
                    if(currentCard >= (totalCards-10)){
                        shuffleDeck(Deck, totalCards );
                        currentCard = 0;
                    }
                    player.aceInHand = 0;
                    dealer.aceInHand = 0;
                    dealCards();
                    checkAce();
                    // Set necessary boolean values
                    nobust = true;
                    doubleDown = false;
                    checkDealerWin(); // Check if dealer is dealt 21
                    blackJack = false;
                    checkBlackjack(); // Check if play
                    hit_num = 1;
                    dealer_hit_num = 1;
                    //displays current total of cards
                    sprintf(statement, “Total: %i”, player.handCount);
                    LCD.SetFontColor(BLACK);
                    LCD.FillRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                    LCD.SetFontColor(LIGHTBLUE);
                    LCD.DrawRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                    text.display(statement, LIGHTBLUE, center(160, 5, 1, strlen(statement)), center(200, 7, 0, 1)+30);
                    LCD.Update();
                    while(playing_state)
                    {
                    //check that player.handcount < 21
                        if(player.handCount < 22){
                        //detect hover
                            while(!LCD.Touch(&x_pos,&y_pos)) {
                            //change color
                                if(x_pos >= 30 && x_pos <= 80 && y_pos >= 100 && y_pos <= 120 ){
                                    player_buttons(hover_col, 100, “Stand”);
                                }else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 130 && y_pos <= 150 ){
                                    player_buttons(hover_col, 130, “Hit”);
                                }else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 160 && y_pos <= 180 && hit_num == 1){
                                    player_buttons(hover_col, 160, “Double”);
                                }else if(hit_num == 1){
                                    player_buttons(menu_button_col, 100, “Stand”); //stand button
                                    player_buttons(menu_button_col, 130, “Hit”); //hit button
                                    player_buttons(menu_button_col, 160, “Double”); //double button
                                }
                                else{
                                    //drawing menu squares and text
                                    player_buttons(menu_button_col, 100, “Stand”); //stand button
                                    player_buttons(menu_button_col, 130, “Hit”); //hit button
                                    player_buttons(DARKRED, 160, “Double”); //double button marked out
                                }   //center(80, 12, 0, 6)
                                LCD.Update();
                            }
                            while(LCD.Touch(&x_pos,&y_pos)){
                                if(x_pos >= 30 && x_pos <= 80 && y_pos >= 100 && y_pos <= 120 ){ //if stand is pressed
                                    Sleep(1500);
                                    dealer.flipCard();
                                    playing_state = false;
                                    round_state = false;
                                }
                                else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 130 && y_pos <= 150 ){ //if hit is pressed
                                    Sleep(1000);
                                    player.hit();
                                    hit_num++;
                                    checkAce();
                                }
                                else if(x_pos >= 30 && x_pos <= 80 && y_pos >= 160 && y_pos <= 180 && wallet >= bet && hit_num == 1){ //if double is pressed
                                        Sleep(1500);
                                        player.hit();
                                        hit_num++;
                                        checkAce();
                                        dealer.flipCard();
                                        blueChipAmount *=2;
                                        greenChipAmount *=2;
                                        redChipAmount *=2;
                                        wallet -= bet;
                                        updateChips();
                                        updateBet();
                                        doubleDown = true;
                                        playing_state = false;
                                        round_state = false;
                                }
                            }
                            sprintf(statement, “Total: %i”, player.handCount); // Display the player’s hand count
                            LCD.SetFontColor(BLACK);
                            LCD.FillRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                            LCD.SetFontColor(LIGHTBLUE);
                            LCD.DrawRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                            text.display(statement, LIGHTBLUE, center(160, 5, 1, strlen(statement)), center(200, 7, 0, 1)+30);
                            LCD.Update();
                        }
                        else{
                            LCD.WriteAt(“You busted!”, center(160, 12, 0, 11),100); // Tell player they are over 21
                            player.losses++;
                            round_state = false;
                            playing_state = false;
                            nobust = false;
                            dealer.flipCard();
                            LCD.Update();
                            totalbet += bet;
                            profit -= bet;
                        }
                    }
                //check if bust on double down
                if(player.handCount > 22){
                    LCD.WriteAt(“You busted!”, center(160, 12, 0, 11),100);
                            player.losses++;
                            round_state = false;
                            playing_state = false;
                            nobust = false;
                            dealer.flipCard();
                            LCD.Update();
                            totalbet += bet;
                            profit -= bet;
                }
                checkAce();
                // Update hand counts
                sprintf(statement, “Total: %i”, player.handCount);
                LCD.SetFontColor(BLACK);
                LCD.FillRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                LCD.SetFontColor(LIGHTBLUE);
                LCD.DrawRectangle(center(160, 60, 0, 1), center(200, 15, 0, 1)+30, 60, 15);
                text.display(statement, LIGHTBLUE, center(160, 5, 1, strlen(statement)), center(200, 7, 0, 1)+30);
                LCD.Update();
                //compare hands to see who won
                if(nobust)
                {
                    if(!blackJack){
                    //do dealer hitting stuff
                    Sleep(1000);
                    dealerHit();
                    LCD.Update();
                    }
                    if(blackJack){
                        LCD.WriteAt(“Blackjack!”, center(160, 12, 0, 8), 100);
                        Sleep(2000);
                        player.wins++;
                        wallet += 2.5*bet;
                        totalbet += bet;
                        profit += 2.5*bet;
                    }
                    if((player.handCount > dealer.handCount) || dealer.handCount > 21) // for a standard player win
                    {
                        LCD.WriteAt(“You win!”, center(160, 12, 0, 8), 100);
                        player.wins++;
                        if (doubleDown)
                        {
                        wallet += 4*bet;
                        updateChips();
                        totalbet += bet;
                        profit += 4*bet;
                        }
                        else
                        {
                        wallet += 2*bet;
                        updateChips();
                        totalbet += bet;
                        profit += 2*bet;
                        }
                        //double money bet
                    }
                    else if (dealer.handCount > player.handCount && dealer.handCount < 21)
                    {
                        LCD.WriteAt(“You lost!”, center(160, 12, 0, 9), 100);
                        player.losses++;
                        totalbet += bet;
                        profit -= bet;
                    }
                    else if(dealer.handCount == player.handCount)
                    {
                        LCD.WriteAt(“Push!”, center(160, 12, 0, 5), 100);
                        player.ties++;
                        wallet += bet;
                        updateChips();
                        totalbet += bet;
                        //give money back
                    }
                }
        }
            //ask if player wants to play again, else exit to menu
            while(!LCD.Touch(&x_pos, &y_pos))
            {
                    LCD.SetFontColor(GRAY);
                    LCD.FillRectangle(250, 1, 69, 25);
                    LCD.SetFontColor(DARKRED);
                    LCD.DrawRectangle(250, 1, 69, 25);
                    LCD.WriteAt(“Exit”, center(250+(69/2.0), 12, 0, 4), center(14, 17, 0, 1));
                    LCD.Update();
                if(x_pos > 250 && x_pos < 319 && y_pos >105 && y_pos < 135 && haschips)
                {
                    LCD.SetFontColor(hover_col);
                    LCD.FillRectangle(250, 105, 69, 30);
                    LCD.SetFontColor(DARKRED);
                    LCD.DrawRectangle(250, 105, 69, 30);
                    text.display(“Play Again”, LIGHTBLUE, center(285, 5, 1, 10), center(120, 7, 0, 1));
                } else if(haschips){
                    LCD.SetFontColor(menu_button_col);
                    LCD.FillRectangle(250, 105, 69, 30);
                    LCD.SetFontColor(DARKRED);
                    LCD.DrawRectangle(250, 105, 69, 30);
                    text.display(“Play Again”, LIGHTBLUE, center(285, 5, 1, 10), center(120, 7, 0, 1));
                }
            }
            while(LCD.Touch(&x_pos, &y_pos))
            {
                if(x_pos > 250 && x_pos < 319 && y_pos > 105 && y_pos < 135)
                {
                    round_state = true;
                } else if(x_pos > 250 && x_pos < 319 && y_pos > 1 && y_pos < 26)
                {
                    state = 0; //exit to menu
                }
            }
        }
    }
    //instructions
    while(state == 2){
        LCD.Clear();
        LCD.Update();
        LCD.WriteAt(“Instructions”, 50, 5);
        text.display(“To start, you will bet however much you please. “, BLACK, 5, 35);
        text.display(“Minimum Bet is 10 Chips. “, BLACK, 5, 45);
        text.display(“Then, two cards will be dealt to you, and two to”, BLACK, 5, 65);
        text.display(“the dealer. You can see one of the dealers cards.”, BLACK, 5, 75);
        text.display(“The objective is to get as close to scoring 21  “, BLACK, 5, 85);
        text.display(“without going over. Numbers are worth their value, “, BLACK, 5, 95);
        text.display(“Ace is worth 1 or 11(whichever makes sense), face “, BLACK, 5, 105);
        text.display(“cards are worth 10. You can hit to gain a card until “, BLACK, 5, 115);
        text.display(“you think you are close. Then the dealer will play. “, BLACK, 5, 125);
        text.display(“The dealer will always hit until their score is >17. “, BLACK, 5, 135);
        text.display(“If you want to double down, hit double down. “, BLACK, 5, 145);
        text.display(“This means you get one more hit and then dealer hits. “, BLACK, 5, 155);
        text.display(“The number of chips paid out/lost is double. “, BLACK, 5, 165);
        text.display(“Enjoy!”, BLACK, 5, 175);
        state = exitButton(state, x_pos, y_pos); //exit button
    }
    //stats
    while(state == 3){
        LCD.Clear();
        LCD.Update();
        LCD.WriteAt(“Total Rounds Played: “, 5, 30);
        LCD.WriteAt((player.wins + player.losses + player.ties), 250, 30);
        LCD.WriteAt(“Rounds Won: “, 5, 55);
        LCD.WriteAt(player.wins,140, 55 );
        LCD.WriteAt(“Rounds Lost: “, 5, 80);
        LCD.WriteAt(player.losses,155, 80 );
        LCD.WriteAt(“Total Amount Bet: “, 5, 105);
        LCD.WriteAt(totalbet,210, 105 );
        LCD.WriteAt(“Profit: “, 5, 130);
        LCD.WriteAt(profit,90, 130);
        state = exitButton(state, x_pos, y_pos); //exit button
    }
    //credits
    while(state == 4){
        LCD.Clear();
        LCD.Update();
        text.display(“Game Developed by James Brickner and James Butts.”, BLACK, 5, 35);
        text.display(“Pixel art and graphics by James and James “, BLACK, 5, 60);
        text.display(“Code written and developed by James”, BLACK, 5, 85);
        text.display(“Copyright 2024 James Inc.”, BLACK, 5, 110);
        text.display(“smaller text font, written by feltlin on github: “, BLACK, 5, 135);
        text.display(“github.com/Feltlin/FEH_Proteus/blob/main/Text.h”, BLACK, 5, 145);
        state = exitButton(state, x_pos, y_pos); //exit button
    }
    }
    return 0;
    }
cards::cards(char a[],char b[])
{
    strcpy(rank, a);
    strcpy(suit, b);
}
Dealer::Dealer()
{
    handCount = 0;
    flippedCard = 0;
}
Player::Player()
{
    handCount = 0;
    wins = 0;
    losses = 0;
}
void menu_button(int button_color, int recty, char name[], int textx, int texty){
    LCD.SetFontColor(button_color);
    LCD.FillRectangle(60, recty, 200, 30);
    LCD.SetFontColor(DARKRED);
    LCD.DrawRectangle(60, recty, 200, 30);
    LCD.WriteAt(name, textx, texty);
}
void betting_buttons(int button_color, int recty, char name[]){
    LCD.SetFontColor(button_color);
    LCD.FillRectangle(30, recty, 50, 20);
    LCD.SetFontColor(DARKRED);
    LCD.DrawRectangle(30, recty, 50, 20);
    text.display(name, DARKRED, center(55, 5, 1, strlen(name)), center(recty+15, 17, 0, 1));
}
void player_buttons(int button_color, int recty, char name[]){
    LCD.SetFontColor(button_color);
    LCD.FillRectangle(30, recty, 50, 20);
    LCD.SetFontColor(DARKRED);
    LCD.DrawRectangle(30, recty, 50, 20);
    text.display(name, DARKRED, center(55, 5, 1, strlen(name)), center(recty+15, 17, 0, 1));
}
float center(float centerx, float length, float spacing, float num){
    //given length of item, spacing between items, number of items, finds spot to start given center
    float tot_dist = (length*num) + ((num-1)*spacing);
    float leftx = centerx-(tot_dist/2.0);
    return leftx;
    /*Example:
    for spacing text, centerx is where you want the center to be
    length is the width of a character(12 pixels)
    spacing is spacing between letters(0 pixels)
    num is number of characters(for Start, 5)
    */
}
void initializeDecks() // Function that initializes the deck of cards
{
    char ranks[13][3] = {“A”, “2”, “3”, “4”, “5”, “6”, “7”, “8”, “9”, “10”, “J”, “Q”, “K”};
    char suits[4][10] = {“Hearts”, “Diamonds”, “Spades”, “Clubs”};
    index = 0;
    for (int i = 0; i < numDecks; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 13; k++)
            {
                strcpy(Deck[index].rank, ranks[k]);
                strcpy(Deck[index].suit, suits[j]);
                index++;
            }
        }
    }
};
void shuffleDeck(cards d[], int totalC) // Function to shuffle the deck of
{
    for (int i = totalC – 1; i > 0; i–)
    {
        int r = rand() % (i + 1); // Generate a random index between 0 and i
        // Swap deck[i] and deck[r]
        cards temp = d[i];
        d[i] = d[r];
        d[r] = temp;
    }
}
void cards :: printCard(int x, int y, int down) // Function that uses the suit and rank of the card to print the right images
{
    char ranks[13][3] = {“A”, “2”, “3”, “4”, “5”, “6”, “7”, “8”, “9”, “10”, “J”, “Q”, “K”};
    char suits[4][10] = {“Hearts”, “Diamonds”, “Spades”, “Clubs”};
    FEHImage baseCard;
    if (!down)
    {
        baseCard.Open(“card_base.png”);
        baseCard.Draw(x,y);
        FEHImage Suit;
        FEHImage Rank;
            if (strcmp(suit, suits[0]) == 0)
            {
                Suit.Open(“heart.png”);
                Suit.Draw(x+30,y+2);
                Suit.Draw(x+2,y+43);
            }
            else if(strcmp(suit, suits[1]) == 0)
            {
                Suit.Open(“diamond.png”);
                Suit.Draw(x+30,y+2);
                Suit.Draw(x+2,y+43);
            }
            else if (strcmp(suit, suits[2]) == 0)
            {
                Suit.Open(“spade.png”);
                Suit.Draw(x+30,y+2);
                Suit.Draw(x+2,y+43);
            }
            else if (strcmp(suit, suits[3]) == 0)
            {
                Suit.Open(“club.png”);
                Suit.Draw(x+30,y+2);
                Suit.Draw(x+2,y+43);
            }
            if(strcmp(suit, suits[0]) == 0 || strcmp(suit, suits[1]) == 0){
                if (strcmp(rank, ranks[0]) == 0)
                {
                    Rank.Open(“ace_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[1]) == 0)
                {
                    Rank.Open(“two_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[2]) == 0)
                {
                    Rank.Open(“three_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+42);
                }
                else if (strcmp(rank, ranks[3]) == 0)
                {
                    Rank.Open(“four_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[4]) == 0)
                {
                    Rank.Open(“five_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[5]) == 0)
                {
                    Rank.Open(“six_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[6]) == 0)
                {
                    Rank.Open(“seven_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+31,y+43);
                }
                else if (strcmp(rank, ranks[7]) == 0)
                {
                    Rank.Open(“eight_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+42);
                }
                else if (strcmp(rank, ranks[8]) == 0)
                {
                    Rank.Open(“nine_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+42);
                }
                else if (strcmp(rank, ranks[9]) == 0)
                {
                    Rank.Open(“ten_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+30,y+43);
                }
                else if (strcmp(rank, ranks[10]) == 0)
                {
                    Rank.Open(“jack_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[11]) == 0)
                {
                    Rank.Open(“queen_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+31,y+43);
                }
                else if (strcmp(rank, ranks[12]) == 0)
                {
                    Rank.Open(“king_red.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
            } else if(strcmp(suit, suits[2]) == 0 || strcmp(suit, suits[3]) == 0){
                    if (strcmp(rank, ranks[0]) == 0)
                {
                    Rank.Open(“ace.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[1]) == 0)
                {
                    Rank.Open(“two.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[2]) == 0)
                {
                    Rank.Open(“three.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+42);
                }
                else if (strcmp(rank, ranks[3]) == 0)
                {
                    Rank.Open(“four.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[4]) == 0)
                {
                    Rank.Open(“five.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[5]) == 0)
                {
                    Rank.Open(“six.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[6]) == 0)
                {
                    Rank.Open(“seven.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+31,y+43);
                }
                else if (strcmp(rank, ranks[7]) == 0)
                {
                    Rank.Open(“eight.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+42);
                }
                else if (strcmp(rank, ranks[8]) == 0)
                {
                    Rank.Open(“nine.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+42);
                }
                else if (strcmp(rank, ranks[9]) == 0)
                {
                    Rank.Open(“ten.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+30,y+43);
                }
                else if (strcmp(rank, ranks[10]) == 0)
                {
                    Rank.Open(“jack.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
                else if (strcmp(rank, ranks[11]) == 0)
                {
                    Rank.Open(“queen.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+31,y+43);
                }
                else if (strcmp(rank, ranks[12]) == 0)
                {
                    Rank.Open(“king.png”);
                    Rank.Draw(x+2,y+2);
                    Rank.Draw(x+32,y+43);
                }
            }
    }
    else
    {
        baseCard.Open(“back_of_card.png”);
        baseCard.Draw(x,y);
    }
}
int convertCards() // Function to convert cards into integer values
{
    int cardValue;
    char ranks[13][3] = {“A”, “2”, “3”, “4”, “5”, “6”, “7”, “8”, “9”, “10”, “J”, “Q”, “K”};
    if (strcmp(Deck[currentCard].rank, ranks[1]) == 0)
    {
        cardValue = 2;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[2]) == 0)
    {
        cardValue = 3;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[3]) == 0)
    {
        cardValue = 4;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[4]) == 0)
    {
        cardValue = 5;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[5]) == 0)
    {
        cardValue = 6;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[6]) == 0)
    {
        cardValue = 7;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[7]) == 0)
    {
        cardValue = 8;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[8]) == 0)
    {
        cardValue = 9;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[9]) == 0 || strcmp(Deck[currentCard].rank, ranks[10]) == 0 || strcmp(Deck[currentCard].rank, ranks[11]) == 0 || strcmp(Deck[currentCard].rank, ranks[12]) == 0)
    {
        cardValue = 10;
    }
    else if (strcmp(Deck[currentCard].rank, ranks[0]) == 0)
    {
        // if ((this.handCount + 11) > 21)
        // {
        //     cardValue = 1;
        // }
        // else
        {
            cardValue = 11;   //add game logic to change to 1 if total > 21
        }
    }
    return cardValue;
}
void dealCards()
{
    dealer.handCount = 0;
    player.handCount = 0;
    Sleep(700);
    Deck[currentCard].printCard(133,160, 0);
    player.handCount += convertCards();
    if (strcmp(Deck[currentCard].rank, “A”) == 0)
    {
        player.aceInHand ++;
    }
    currentCard++;
    Sleep(700);
    Deck[currentCard].printCard(133,95, 0);
    dealer.handCount += convertCards();
    if (strcmp(Deck[currentCard].rank, “A”) == 0)
    {
        dealer.aceInHand ++;
    }
    currentCard++;
    dealertotal();
    Sleep(700);
    Deck[currentCard].printCard(143,160, 0);
    player.handCount += convertCards();
    if (strcmp(Deck[currentCard].rank, “A”) == 0)
    {
        player.aceInHand++;
    }
    currentCard++;
    Sleep(700);
    Deck[currentCard].printCard(143,95, 1);
    dealer.handCount += convertCards();
    dealer.flippedCard = currentCard;
    if (strcmp(Deck[currentCard].rank, “A”) == 0)
    {
        dealer.aceInHand++;
    }
    currentCard++;
}
void checkAce() //check if player or dealer has an ace, to know whether or not to decrease the value of their hand
{
    if (player.aceInHand > 0 && player.handCount > 21)
    {
        player.handCount -= 10;
        player.aceInHand–;
    }
    if (dealer.aceInHand > 0 && dealer.handCount > 21)
    {
        dealer.handCount -= 10;
        dealer.aceInHand–;
    }
}
void checkDealerWin()  //to check if dealer wins off the deal
{
    if(dealer.handCount == 21 && player.handCount != 21){
        Sleep(1000);
        dealer.flipCard();
        playing_state = false;
        text.display(“Dealer Blackjack, You Lose!”, RED, center(160, 12, 0, 11), 100);
        player.losses++;
    }
    //take away money
}
void checkBlackjack()
{
   if(player.handCount == 21 && dealer.handCount != 21)
   {
        dealer.flipCard();
        playing_state = false;
        blackJack = true;
   }
}
void Player :: hit(){  //function for when player hits
    Deck[currentCard].printCard(143+(10*hit_num),160, 0);
    handCount += convertCards();
    if (strcmp(Deck[currentCard].rank, “A”) == 0)
    {
        player.aceInHand++;
    }
    currentCard++;
}
void Dealer :: flipCard() //function to flip hidden card when player stands
{
    Deck[flippedCard].printCard(143,95, 0);
    dealertotal();
}
void dealerHit(){  //function to automate dealer hitting9
    while(dealer.handCount < 17)
    {
        Deck[currentCard].printCard(143+(10*dealer_hit_num),95, 0);
        dealer.handCount += convertCards();
        if (strcmp(Deck[currentCard].rank, “A”) == 0)
        {
        dealer.aceInHand++;
        }
        checkAce();
        currentCard++;
        dealer_hit_num++;
        dealertotal();
        Sleep(1000);
    }
}
void playertotal(){
}
void dealertotal(){
    sprintf(dealerhandtotal, “Total: %i”, dealer.handCount);
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(center(160, 60, 0, 1), center(10, 15, 0, 1), 60, 15);
    LCD.SetFontColor(LIGHTBLUE);
    LCD.DrawRectangle(center(160, 60, 0, 1), center(10, 15, 0, 1), 60, 15);
    text.display(dealerhandtotal, LIGHTBLUE, center(160, 5, 1, strlen(dealerhandtotal)), center(10, 7, 0, 1));
    LCD.Update();
}
void updateBet()
{
    fiftyChip.Open(“blue_chip.png”);
    twentyChip.Open(“green_chip.png”);
    tenChip.Open(“red_chip.png”);
    int chipx = 280;
    int chipy = 170;
    for (int i = 0; i < blueChipAmount; i++)
    {
        fiftyChip.Draw(chipx, chipy-(4*(i+1)));
    }
    chipx = 260;
    chipy = 170;
    for (int i = 0; i < greenChipAmount; i++)
    {
        twentyChip.Draw(chipx, chipy-(4*(i+1)));
    }
    chipx = 240;
    chipy = 170;
    for (int i = 0; i < redChipAmount; i++)
    {
        tenChip.Draw(chipx, chipy-(4*(i+1)));
    }
}
void updateChips() //updates total in wallet
{
    sprintf(walletchar, “Wallet: %i Chips”, wallet);
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(5, center(200, 15, 0, 1)+30, 110, 15);
    LCD.SetFontColor(LIGHTBLUE);
    LCD.DrawRectangle(5, center(200, 15, 0, 1)+30, 110, 15);
    text.display(walletchar, LIGHTBLUE, center(60, 5, 1, strlen(walletchar)), center(200, 7, 0, 1)+30);
    LCD.Update();
}
void payout(float bet){  //handles paying out dependent on win
}
int exitButton(int state, float x_pos, float y_pos){  //creates exit button with functionality
        //exit to menu button
        LCD.SetFontColor(GRAY);
        LCD.FillRectangle(250, 1, 69, 25);
        LCD.SetFontColor(DARKRED);
        LCD.DrawRectangle(250, 1, 69, 25);
        LCD.WriteAt(“Exit”, center(250+(69/2.0), 12, 0, 4), center(14, 17, 0, 1));
        LCD.Update();
        while(!LCD.Touch(&x_pos,&y_pos)) {}
        while(LCD.Touch(&x_pos,&y_pos)) {}
        if(x_pos > 250 && x_pos < 319 && y_pos > 1 && y_pos < 26){
            state = 0;  //exit to menu
        }
        LCD.Update();
        return state;
}