from django.db import models
from django.utils import timezone

# Create your models here.


class Player(models.Model):
    name = models.CharField(primary_key = True, max_length = 100)
    health = models.IntegerField(default = 100)
    hunger = models.IntegerField(default = 0)
    major = models.IntegerField(default = 6)
    minor = models.IntegerField(default=0)
    happiness = models.IntegerField(default = -20)
    gpa = models.FloatField(default = 5.0)
    gpa_slider = models.IntegerField(default = 0)
    units = models.IntegerField(default = 0)
    activity = models.CharField(max_length = 100)
    last_seen = models.DateTimeField(default=timezone.now)


class Course(models.Model):
    person = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='person')
    course = models.CharField(max_length=100)


class InfiniteScore(models.Model):
    person = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='player', primary_key=True)
    score = models.IntegerField(default = 0)
