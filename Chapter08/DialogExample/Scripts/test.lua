intro = {
	say = 'Hello I am the Helper NPC, can I help you?',
	choices = {
                 choice1 = "Yes! I need help",
                 choice2 = "No!! Leave me alone"
    },
	
	onSelection = function (choice) 
		if choice == CurrentDialog["choices"]["choice1"] then CurrentDialog = getHelp end
		if choice  == CurrentDialog["choices"]["choice2"] then CurrentDialog = goodbye_mean end
	end
}

getHelp = {
	say = 'Ok I am still working on my helpfulness',
	choices = {
                 choice1 = "That's okay! Thank you!",
                 choice2 = "That's weak, what a waste!",
				 choice3 = "Start over please."
        },
	onSelection = function (choice) 
		if choice  == CurrentDialog["choices"]["choice1"] then CurrentDialog = goodbye 
		elseif choice  == CurrentDialog["choices"]["choice2"] then CurrentDialog = goodbye_mean 
		elseif choice  == CurrentDialog["choices"]["choice3"] then CurrentDialog = intro end
	end

}

goodbye = {
	say = "See you soon, goodbye!"
}

goodbye_mean = {
	say = "Wow that is mean, goodbye!"
}

CurrentDialog = intro
